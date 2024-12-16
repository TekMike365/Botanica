import sys
import os

SHADERS_DIR = "src/shaders/"
SHADER_EXT = "glsl"
SHADERS_HEADER = "src/shaders/Shaders.h"

files = [ f for f in os.listdir(SHADERS_DIR)
         if os.path.isfile(SHADERS_DIR + f) and os.path.splitext(f)[-1] == "." + SHADER_EXT ]

print("Embedding...")

out = open(SHADERS_HEADER, "w")
out = open(SHADERS_HEADER, "a")
out.write("#pragma once\n\n")

for f in files:
    print(f)
    name = os.path.splitext(f)[0]
    src = open(SHADERS_DIR + f, "r").read()
    out.write(f"inline const char *{name}_{SHADER_EXT} = R\"(\n{src})\";\n\n")

out.close()

print("Done.")