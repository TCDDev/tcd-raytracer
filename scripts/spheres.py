import json

materials = ["red", "green", "blue", "yellow"]

spheres = []

rows = 10
cols = 10

spacing = 1.5
radius = 0.5

start_x = -((cols - 1) * spacing) / 2
start_z = -((rows - 1) * spacing) / 2

for row in range(rows):
    for col in range(cols):
        x = start_x + col * spacing
        z = start_z + row * spacing

        spheres.append({
            "type": "sphere",
            "center": [x, radius - 1.0, z],
            "radius": radius,
            "material": materials[(row + col) % len(materials)]
        })

print(json.dumps(spheres, indent=2))