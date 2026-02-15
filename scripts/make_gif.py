import glob
from PIL import Image

def make_gif():
    # 1. Find all files output_000.ppm, output_001.ppm, etc.
    files = sorted(glob.glob("output_*.ppm"))
    
    if not files:
        print("No PPM files found! Run the engine first.")
        return

    print(f"Found {len(files)} frames. Creating GIF...")

    images = [Image.open(f) for f in files]

    # 2. Save as Animated GIF
    images[0].save(
        "animation.gif",
        save_all=True,
        append_images=images[1:],
        duration=160, # 200ms per frame = 20 FPS
        loop=0
    )
    print("Done! Open 'animation.gif' to see your physics!")

if __name__ == "__main__":
    make_gif()