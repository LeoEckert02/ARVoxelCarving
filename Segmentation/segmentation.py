import os
import cv2
import numpy as np
from sam2.build_sam import build_sam2
from sam2.sam2_image_predictor import SAM2ImagePredictor
from PIL import Image

# Global variables to store user interactions
current_image_index = 0
bounding_boxes = []
selected_points = []

def load_image_with_box(image, window_name="Select Object"):
    """
    Allow the user to draw a bounding box on the image in the same window.
    """
    global bounding_boxes
    bounding_box = []
    drawing = False
    start_point = (0, 0)

    def draw_rectangle(event, x, y, flags, param):
        nonlocal start_point, drawing, bounding_box
        if event == cv2.EVENT_LBUTTONDOWN:
            drawing = True
            start_point = (x, y)
        elif event == cv2.EVENT_MOUSEMOVE:
            if drawing:
                temp_image = display_image.copy()
                cv2.rectangle(temp_image, start_point, (x, y), (0, 255, 0), 2)
                cv2.imshow(window_name, temp_image)
        elif event == cv2.EVENT_LBUTTONUP:
            drawing = False
            end_point = (x, y)
            bounding_box = [min(start_point[0], end_point[0]), min(start_point[1], end_point[1]),
                            max(start_point[0], end_point[0]), max(start_point[1], end_point[1])]
            cv2.rectangle(display_image, start_point, end_point, (0, 255, 0), 2)
            cv2.imshow(window_name, display_image)
            bounding_boxes.append(bounding_box)
            return True  # Indicate that the interaction is complete

    # Resize the image for display
    screen_width, screen_height = 800, 800
    h, w, _ = image.shape
    scale = max(w / screen_width, h / screen_height, 1.0)
    resized_w, resized_h = int(w / scale), int(h / scale)
    display_image = cv2.resize(image, (resized_w, resized_h), interpolation=cv2.INTER_AREA)
    scale_w, scale_h = w / resized_w, h / resized_h

    cv2.imshow(window_name, display_image)
    cv2.setMouseCallback(window_name, draw_rectangle)

    while True:
        key = cv2.waitKey(1) & 0xFF
        if bounding_box:  # If the user has drawn a box, move to the next image
            break
        if key == ord('q'):  # Allow the user to quit
            break

    # Scale the bounding box back to the original image size
    if bounding_box:
        bounding_box = [int(coord * scale_w) if i % 2 == 0 else int(coord * scale_h) for i, coord in enumerate(bounding_box)]
    return bounding_box

def load_image_click(image, window_name="Select Object"):
    """
    Allow the user to click on the image in the same window.
    """
    global selected_points
    selected_point = []

    def click_event(event, x, y, flags, param):
        if event == cv2.EVENT_LBUTTONDOWN:
            orig_x = int(x * scale_w)
            orig_y = int(y * scale_h)
            selected_point.append((orig_x, orig_y))
            cv2.circle(display_image, (x, y), 5, (0, 0, 255), -1)  # Draw a dot where clicked
            cv2.imshow(window_name, display_image)
            selected_points.append((orig_x, orig_y))
            return True  # Indicate that the interaction is complete

    # Resize the image for display
    screen_width, screen_height = 800, 800
    h, w, _ = image.shape
    scale_w = w / screen_width if w > screen_width else 1.0
    scale_h = h / screen_height if h > screen_height else 1.0
    scale = max(scale_w, scale_h)
    resized_w, resized_h = int(w / scale), int(h / scale)
    display_image = cv2.resize(image, (resized_w, resized_h), interpolation=cv2.INTER_AREA)

    cv2.imshow(window_name, display_image)
    cv2.setMouseCallback(window_name, click_event)

    while True:
        key = cv2.waitKey(1) & 0xFF
        if selected_point:  # If the user has clicked, move to the next image
            break
        if key == ord('q'):  # Allow the user to quit
            break

    return selected_point[0] if selected_point else None

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
        self.all_scores = []
        self.all_masked_images = []

    def load_images(self, folder_path):
        """Load images from the specified folder."""
        image_paths = [os.path.join(folder_path, file) for file in os.listdir(folder_path)
                       if file.endswith(('.png', '.jpg', '.jpeg', '.JPG'))]
        images = [Image.open(image_path) for image_path in image_paths]
        images.sort(key=lambda f: int(''.join(filter(str.isdigit, os.path.basename(f.filename)))))
        self.images = [np.array(img.convert('RGB')) for img in images]
        if self.needs_input:
            self.load_all_images_clicks()

    def load_all_images_clicks(self):
        """Handle all user interactions in a single window."""
        global current_image_index, bounding_boxes, selected_points
        window_name = "Select Object"
        cv2.namedWindow(window_name, cv2.WINDOW_NORMAL)

        for i, img in enumerate(self.images):
            print(f"Processing image {i + 1}/{len(self.images)}")
            if self.needs_bounding_box:
                box = load_image_with_box(img, window_name)
                self.boxes.append(box)
            else:
                point = load_image_click(img, window_name)
                if point:
                    self.points.append([point])
                    self.labels.append([1])  # Assume label 1 for all points

        cv2.destroyAllWindows()  # Close the window after all interactions

    def predict(self):
        """Run prediction for each image using user-selected points or boxes."""
        self.predictor.set_image_batch(self.images)
        if self.needs_bounding_box:
            boxes_batch = [np.array([box], dtype=float) for box in self.boxes]
            self.all_masked_images, self.all_scores, _ = self.predictor.predict_batch(box_batch=boxes_batch)
        else:
            point_coords_batch = [np.array([point], dtype=int) for point in self.points]
            labels_batch = [np.array([label], dtype=int) for label in self.labels]
            self.all_masked_images, self.all_scores, _ = self.predictor.predict_batch(
                point_coords_batch=point_coords_batch,
                point_labels_batch=labels_batch)

    def save_segmented_images(self, output_path, show=False):
        """Save the segmented images."""
        for i, (image, masks, scores) in enumerate(zip(self.images, self.all_masked_images, self.all_scores)):
            print(f"Saving image {i + 1}/{len(self.images)}")
            save_segmented_image(image, masks, scores, os.path.join(output_path, f"segmented_image_{i}.png"), show)

def save_segmented_image(image, masks, scores, output_path, show):
    """Save the segmented image."""
    sorted_ind = np.argsort(scores)[::-1]
    masks = masks[sorted_ind]
    scores = scores[sorted_ind]
    best_mask = masks[0]
    h, w, _ = image.shape
    rgba_image = np.zeros((h, w, 4), dtype=np.uint8)
    rgba_image[:, :, :3] = image
    rgba_image[:, :, 3] = (best_mask * 255).astype(np.uint8)
    boolean_mask = best_mask > 0.5
    masked_image = np.zeros_like(rgba_image)
    masked_image[boolean_mask] = rgba_image[boolean_mask]
    Image.fromarray(masked_image, "RGBA").save(output_path)
    if show:
        Image.fromarray(masked_image, "RGBA").show()
    print(f"Score: {scores[0]}")

# Example usage
# sam2_segmentation = SAM2Segmentation(checkpoint='path_to_checkpoint', model_cfg='path_to_model_cfg', device='cuda')
# sam2_segmentation.load_images('path_to_images')
# sam2_segmentation.predict()
# sam2_segmentation.save_segmented_images('output_path')