﻿import sys

from segmentation import SAM2Segmentation


def main(needs_input=True, needs_bounding_box=True, show_results=False):
    # Parameters
    checkpoint = "../Segmentation/sam2.1_model/sam2.1_hiera_large.pt"
    model_cfg = "configs/sam2.1/sam2.1_hiera_l.yaml"
    input_images_folder = "../resources/u_input/"
    segmented_images_folder = "../resources/u_segmented_output/"

    # Print where are we in the directory now
    # Initialize SAM2 model
    segmentator = SAM2Segmentation(checkpoint, model_cfg, 'cpu', needs_input, needs_bounding_box)
    segmentator.load_images(input_images_folder)
    segmentator.predict()
    segmentator.save_segmented_images(segmented_images_folder, show_results)


if __name__ == "__main__":
    needs_input = True
    needs_bounding_box = True
    show_results = False
    if len(sys.argv) > 1:
        needs_input = sys.argv[1].lower() == "true"
    if len(sys.argv) > 2:
        needs_bounding_box = sys.argv[2].lower() == "true"
    if len(sys.argv) > 3:
        show_results = sys.argv[3].lower() == "true"

    main(needs_input, needs_bounding_box, show_results)
