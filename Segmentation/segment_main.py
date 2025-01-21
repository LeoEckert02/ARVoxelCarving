import torch

from segmentation import SAM2Segmentation


def main():
    # Parameters
    checkpoint = "sam2.1_model/sam2.1_hiera_large.pt"
    model_cfg = "configs/sam2.1/sam2.1_hiera_l.yaml"
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    input_images_folder = "../resources/input_images"
    segmented_images_folder = "../resources/segmented_images/"
    needs_input = True
    needs_bounding_box = True
    # Initialize SAM2 model
    segmentator = SAM2Segmentation(checkpoint, model_cfg, device, needs_input, needs_bounding_box)
    segmentator.load_images(input_images_folder)
    segmentator.predict()
    segmentator.show_predicted_images()


if __name__ == "__main__":
    main()
