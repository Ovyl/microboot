#!/bin/python3

import argparse
import zlib

IMG_TOOL_HEADER_MAGIC = 0xE5B0074E
IMG_TOOL_FOOTER_MAGIC = 0xE5B007F0
IMAGE_TOOL_HEADER_SIZE = 0x200

class img_util_opts:
    endianness = 'little'

class img_header:
    magic = IMG_TOOL_HEADER_MAGIC
    image_size = 0
    version_major = 0
    version_minor = 0
    version_patch = 0

    def to_bytes(self, endianness):
        buffer = bytes()
        buffer += self.magic.to_bytes(4, endianness)
        buffer += self.image_size.to_bytes(4, endianness)
        buffer += self.version_major.to_bytes(2, endianness)
        buffer += self.version_minor.to_bytes(2, endianness)
        buffer += self.version_patch.to_bytes(2, endianness)
        buffer += bytes(IMAGE_TOOL_HEADER_SIZE - 14)
        return buffer

class img_footer:
    magic = IMG_TOOL_FOOTER_MAGIC
    crc32 = 0

    def to_bytes(self, endianness):
        buffer = bytes()
        buffer += self.magic.to_bytes(4, endianness)
        buffer += self.crc32.to_bytes(4, endianness)
        return buffer

class img_util:

    # Set defaults
    options = img_util_opts()
    img_header = img_header()
    img_footer = img_footer()
    output_image = bytes()

    def __init__(self):
        # Setup argument parsing
        self.setup_parser()
        self.args = self.parser.parse_args()

        # Parse arguments
        self.parse_arguments()

        # Read input image
        self.read_input_file()

        # Generate image
        self.generate_header()
        self.output_image += self.input_image
        self.generate_footer()

        self.write_output_image()

    def die(self, msg, exit_code = 1):
        print(f"Error: {msg}")
        exit(exit_code)

    # Setup argument parser
    def setup_parser(self):
        self.parser = argparse.ArgumentParser(
                    prog='img_util',
                    description='Tool to generate MicroBoot image.',
                    epilog='')

        self.parser.add_argument("in_file", help="Input binary image.")
        self.parser.add_argument("out_file", help="Output MicroBoot image.")
        self.parser.add_argument("-e", "--endianness", type=str, help="Endianness [little|big] (default: 'little')")

        self.parser.add_argument("--major", type=int, help="Version Major")
        self.parser.add_argument("--minor", type=int, help="Version Minor")
        self.parser.add_argument("--patch", type=int, help="Version Patch")

    # Process arguments
    def parse_arguments(self):

        # Check endianness argument
        if self.args.endianness:
            if self.args.endianness != "little" and self.args.endianness != "big":
                self.die("Endianness must be 'little' or 'big'.")
            self.options.endianness = self.args.endianness

    def read_input_file(self):
        try:
            file = open(self.args.in_file, "rb")
            self.input_image = file.read()
        except:
            self.die("Unable to open input file.")

    def generate_header(self):
        self.img_header.image_size = len(self.input_image)

        self.img_header.version_major = self.args.major if self.args.major  else 0
        self.img_header.version_minor = self.args.minor if self.args.minor  else 0
        self.img_header.version_patch = self.args.patch if self.args.patch  else 0
        self.output_image += self.img_header.to_bytes(self.options.endianness)

    def generate_footer(self):
        self.img_footer.crc32 = zlib.crc32(self.input_image)
        self.output_image += self.img_footer.to_bytes(self.options.endianness)

    def write_output_image(self):
        try:
            file = open(self.args.out_file, "wb")
            file.write(self.output_image)
        except Exception as e:
            self.die(f"Unable to write output file. {e}")

if __name__ == "__main__":
    img_util()
