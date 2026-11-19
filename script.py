from PIL import Image

def convert_to_ppm(input_path, output_path, max_width=1200, max_height=850):
    img = Image.open(input_path).convert("RGB")

    original_width, original_height = img.size

    # Resize the image while maintaining its aspect ratio
    img.thumbnail((max_width, max_height), Image.Resampling.LANCZOS)

    width, height = img.size

    with open(output_path, "w", encoding="utf-8") as file:
        file.write(f"P3\n{width} {height}\n255\n")

        row_buffer = []

        for r, g, b in img.getdata():
            row_buffer.append(f"{r} {g} {b}")

            if len(row_buffer) == width:
                file.write("    ".join(row_buffer) + "\n")
                row_buffer = []

    print(f"Conversion complete!")
    print(f"Original size: {original_width} x {original_height}")
    print(f"New size: {width} x {height}")
    print(f"Saved as: {output_path}")


# Put your image location and output filename here
input_path = "mountain.jpg"
output_path = "output_mountain.ppm"

convert_to_ppm(input_path, output_path)