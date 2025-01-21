import os
import cv2
import numpy as np
import torch
from torchvision.utils import draw_bounding_boxes

from sam2.build_sam import build_sam2
from sam2.sam2_image_predictor import SAM2ImagePredictor
from PIL import Image


def load_image_with_box(image):
    bounding_box = []

    def draw_rectangle(event, x, y, flags, param):
        nonlocal start_point, drawing, bounding_box
        if event == cv2.EVENT_LBUTTONDOWN:
            # Start drawing the bounding box
            drawing = True
            start_point = (x, y)

        elif event == cv2.EVENT_MOUSEMOVE:
            if drawing:
                # Update the rectangle as the user drags
                temp_image = display_image.copy()
                cv2.rectangle(temp_image, start_point, (x, y), (0, 255, 0), 2)
                cv2.imshow("Select Object with Box", temp_image)

        elif event == cv2.EVENT_LBUTTONUP:
            # Finish drawing the bounding box
            drawing = False
            end_point = (x, y)
            bounding_box = [min(start_point[0], end_point[0]), min(start_point[1], end_point[1]),
                            max(start_point[0], end_point[0]), max(start_point[1], end_point[1])]
            cv2.rectangle(display_image, start_point, end_point, (0, 255, 0), 2)
            cv2.imshow("Select Object with Box", display_image)
            cv2.waitKey(500)
            cv2.destroyAllWindows()

    # Resize the image for display
    screen_width, screen_height = 800, 800
    h, w, _ = image.shape
    scale = max(w / screen_width, h / screen_height, 1.0)
    resized_w, resized_h = int(w / scale), int(h / scale)
    display_image = cv2.resize(image, (resized_w, resized_h), interpolation=cv2.INTER_AREA)
    scale_w, scale_h = w / resized_w, h / resized_h

    start_point = (0, 0)
    drawing = False

    # Show the image and set mouse callback
    cv2.imshow("Select Object with Box", display_image)
    cv2.setMouseCallback("Select Object with Box", draw_rectangle)
    cv2.waitKey(0)

    # Scale the bounding box back to the original image size
    if bounding_box:
        bounding_box = [int(coord * scale_w) if i % 2 == 0 else int(coord * scale_h) for i, coord in
                        enumerate(bounding_box)]

    return bounding_box


def load_image_click(image):
    selected_point = []

    def click_event(event, x, y, flags, param):
        if event == cv2.EVENT_LBUTTONDOWN:
            # Scale the selected coordinates back to the original image size
            orig_x = int(x * scale_w)
            orig_y = int(y * scale_h)
            selected_point.append((orig_x, orig_y))
            cv2.circle(display_image, (x, y), 5, (0, 0, 255), -1)  # Draw a dot where clicked
            cv2.imshow("Select Object", display_image)
            cv2.waitKey(500)  # Wait briefly to allow the user to see the dot
            cv2.destroyAllWindows()  # Close the window

    screen_width = 800  # Max width to fit
    screen_height = 800  # Max height to fit

    h, w, _ = image.shape
    scale_w = w / screen_width if w > screen_width else 1.0
    scale_h = h / screen_height if h > screen_height else 1.0
    scale = max(scale_w, scale_h)
    resized_w, resized_h = int(w / scale), int(h / scale)

    display_image = cv2.resize(image, (resized_w, resized_h), interpolation=cv2.INTER_AREA)

    # Show the resized image
    cv2.imshow("Select Object", display_image)
    cv2.setMouseCallback("Select Object", click_event)
    cv2.waitKey(0)
    return selected_point[0] if selected_point else None


def show_segmented_image(image, masks, scores):
    sorted_ind = np.argsort(scores)[::-1]
    masks = masks[sorted_ind]
    scores = scores[sorted_ind]

    # for i in range(len(masks)):
    h, w, _ = image.shape
    rgba_image = np.zeros((h, w, 4), dtype=np.uint8)
    rgba_image[:, :, :3] = image
    rgba_image[:, :, 3] = (masks[0] * 255).astype(np.uint8)
    Image.fromarray(rgba_image, "RGBA").show()
    print(scores[0])


class SAM2Segmentation:
    def __init__(self, checkpoint, model_cfg, device, needs_input=True, needs_bounding_box=False):
        self.device = device
        self.model = build_sam2(model_cfg, ckpt_path=checkpoint, device=device)
        self.predictor = SAM2ImagePredictor(self.model)
        self.images = []
        self.points = []
        self.labels = []
        self.boxes = []
        self.needs_input = needs_input
        self.needs_bounding_box = needs_bounding_box
        # Output
        self.all_scores = []
        self.all_masked_images = []

    def load_images(self, folder_path):
        """Load images from the specified folder."""
        image_paths = [os.path.join(folder_path, file) for file in os.listdir(folder_path)
                       if file.endswith(('.png', '.jpg', '.jpeg'))]
        images = [Image.open(image_path) for image_path in image_paths]
        self.images = [np.array(img.convert('RGB')) for img in images]
        # self.points = np.zeros((len(self.images), 2), dtype=int)
        # self.labels = np.zeros(len(self.images), dtype=int)

        if self.needs_input:
            self.load_all_images_clicks()

    def load_all_images_clicks(self):
        for i, img in enumerate(self.images):
            points_for_image = []
            labels_for_image = []
            if self.needs_bounding_box:
                box = load_image_with_box(img)
                self.boxes.append(box)
            else:
                for j in range(2):  # Collect 2 points for each image
                    point = load_image_click(img)
                    if point:
                        points_for_image.append(point)
                        labels_for_image.append(1)  # Assume label 1 for all points

                self.points.append(points_for_image)
                self.labels.append(labels_for_image)

    def predict(self):
        """
        Run prediction for each image using user-selected points.
        `points` is a list of coordinate tuples (x, y) for each image.
        """

        self.predictor.set_image_batch(self.images)

        if self.needs_bounding_box:
            boxes_batch = [np.array([box], dtype=float) for box in self.boxes]

            self.all_masked_images, self.all_scores, _ = self.predictor.predict_batch(
                box_batch=boxes_batch) if self.needs_input else self.predictor.predict_batch()
        else:
            point_coords_batch = [np.array([point], dtype=int) for point in self.points]
            labels_batch = [np.array([label], dtype=int) for label in self.labels]

            self.all_masked_images, self.all_scores, _ = self.predictor.predict_batch(
                point_coords_batch=point_coords_batch,
                point_labels_batch=labels_batch) if self.needs_input else self.predictor.predict_batch()

    def show_predicted_images(self):
        for i, (image, masks, scores) in enumerate(zip(self.images, self.all_masked_images, self.all_scores)):
            print(f"Image {i + 1}/{len(self.images)}")
            show_segmented_image(image, masks, scores)


def save_segmented_image(image, mask, output_path):
    """Save the segmented image with a mask applied."""
    h, w, _ = image.shape
    rgba_image = np.zeros((h, w, 4), dtype=np.uint8)
    rgba_image[:, :, :3] = image
    rgba_image[:, :, 3] = mask * 255
    Image.fromarray(rgba_image, "RGBA").save(output_path)
