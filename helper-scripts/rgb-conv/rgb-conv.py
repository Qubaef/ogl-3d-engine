def rgb_percent_to_rgb_capped(rgb_percent):
    """
    Convert a percentage of RGB to a capped RGB value.
    """
    return [int(x * 255) for x in rgb_percent]


def rgb_capped_to_rgb_percent(rgb_capped):
    """
    Convert a capped RGB value to a percentage of RGB.
    """

    precision_places = 2
    return [round(x / 255, precision_places) for x in rgb_capped]


def trash_string_to_float(trash_string):
    """
    Convert a string with trashy separators to a float.
    """
    clean_string = trash_string.replace(',', '')
    clean_string = clean_string.replace(' ', '')

    return float(clean_string)


if __name__ == '__main__':
    # Get program arguments
    import argparse

    parser = argparse.ArgumentParser(description='Convert between RGB and percentage of RGB.')
    parser.add_argument('--rgb', type=str, nargs=3, metavar=('R', 'G', 'B'),
                        help='RGB values to convert')
    parser.add_argument('--rgb-percent', type=str, nargs=3, metavar=('R', 'G', 'B'),
                        help='Percentage of RGB values to convert')
    args = parser.parse_args()

    # Convert RGB to percentage of RGB
    if args.rgb:
        # Extract floats from strings
        rgb = [trash_string_to_float(x) for x in args.rgb]
        print(rgb_capped_to_rgb_percent(rgb))
    elif args.rgb_percent:
        # Extract floats from strings
        rgb_percent = [trash_string_to_float(x) for x in args.rgb_percent]
        print(rgb_percent_to_rgb_capped(rgb_percent))
