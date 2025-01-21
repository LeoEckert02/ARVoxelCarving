import os
import cv2
import numpy as np
import torch
from sam2.build_sam import build_sam2
from sam2.sam2_image_predictor import SAM2ImagePredictor
from PIL import Image


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

    for i in range(len(masks)):
        h, w, _ = image.shape
        rgba_image = np.zeros((h, w, 4), dtype=np.uint8)
        rgba_image[:, :, :3] = image
        rgba_image[:, :, 3] = (masks[i] * 255).astype(np.uint8)
        Image.fromarray(rgba_image, "RGBA").show()
        print(scores[i])


class SAM2Segmentation:
    def __init__(self, checkpoint, model_cfg, device, needs_input=True):
        self.device = device
        self.model = build_sam2(model_cfg, checkpoint)
        self.predictor = SAM2ImagePredictor(self.model)
        self.images = []
        self.points = np.array([])
        self.labels = np.array([])
        self.needs_input = needs_input
        # Output
        self.all_scores = []
        self.all_masked_images = []

    def load_images(self, folder_path):
        """Load images from the specified folder."""
        image_paths = [os.path.join(folder_path, file) for file in os.listdir(folder_path)
                       if file.endswith(('.png', '.jpg', '.jpeg'))]
        images = [Image.open(image_path) for image_path in image_paths]
        self.images = [np.array(img.convert('RGB')) for img in images]
        self.points = np.zeros((len(self.images), 2), dtype=int)
        self.labels = np.zeros(len(self.images), dtype=int)

        if self.needs_input:
            self.load_all_images_clicks()

    def load_all_images_clicks(self):
        for i, img in enumerate(self.images):
            self.points[i] = load_image_click(img)
            self.labels[i] = 1

    def predict(self):
        """
        Run prediction for each image using user-selected points.
        `points` is a list of coordinate tuples (x, y) for each image.
        """
        point_coords_batch = [np.array([point], dtype=int) for point in self.points]
        labels_batch = [np.array([label], dtype=int) for label in self.labels]

        self.predictor.set_image_batch(self.images)
        self.all_masked_images, self.all_scores, _ = self.predictor.predict_batch(point_coords_batch,
                                                                                  labels_batch) if self.needs_input else self.predictor.predict_batch()

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

# def main():
#     # Parameters
#     checkpoint = "sam2.1_model/sam2.1_hiera_large.pt"
#     model_cfg = "configs/sam2.1/sam2.1_hiera_l.yaml"
#     device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
#     input_images_folder = "../resources/input_images"
#     segmented_images_folder = "../resources/segmented_images/"
#
#     # Initialize SAM2 model
#     segmenter = SAM2Segmentation(checkpoint, model_cfg, device)
#
#     # Load images
#     images, image_paths = segmenter.load_images(input_images_folder)
#
#     # Collect user-selected points
#     user_points = []
#     for i, image in enumerate(images):
#         print(f"Processing Image {i + 1}/{len(images)}: {os.path.basename(image_paths[i])}")
#         selected_point = select_object_opencv(image)
#         if selected_point:
#             print(f"Selected Point for Image {i + 1}: {selected_point}")
#             user_points.append(selected_point)
#         else:
#             print(f"No point selected for Image {i + 1}. Skipping...")
#             user_points.append(None)  # Mark as None for skipping prediction
#
#     # Filter images and points where the user selected a point
#     filtered_images = [img for img, pt in zip(images, user_points) if pt is not None]
#     filtered_points = [pt for pt in user_points if pt is not None]
#
#     # Predict masks for all valid images
#     all_masks, all_scores, _ = segmenter.predict(filtered_images, filtered_points)
#
#     # Save results
#     for i, (image, masks, scores, path) in enumerate(zip(filtered_images, all_masks, all_scores, image_paths)):
#         # Sort masks by score
#         sorted_ind = np.argsort(scores)[::-1]
#         masks = masks[sorted_ind]
#
#         # Save the top mask
#         output_path = os.path.join(segmented_images_folder, f"segmented_{os.path.basename(path)}")
#         save_segmented_image(image, masks[0], output_path)
#         print(f"Segmented image saved: {output_path}")
