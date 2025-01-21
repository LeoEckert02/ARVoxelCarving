from segmentation import SAM2Segmentation

needs_input = True
needs_bounding_box = True
show_results = False


def main():
    # Parameters
    checkpoint = "sam2.1_model/sam2.1_hiera_large.pt"
    model_cfg = "configs/sam2.1/sam2.1_hiera_l.yaml"
    input_images_folder = "../resources/input_images"
    segmented_images_folder = "../resources/segmented_images/"

    # Initialize SAM2 model
    segmentator = SAM2Segmentation(checkpoint, model_cfg, 'cpu', needs_input, needs_bounding_box)
    segmentator.load_images(input_images_folder)
    segmentator.predict()
    segmentator.save_segmented_images(segmented_images_folder, show_results)


if __name__ == "__main__":
    main()
