# save as generate_circle_pbm.py
W, H = 32, 32
R = 13  # радиус

with open("circle32.pbm", "w") as f:
    f.write("P1\n")
    f.write(f"{W} {H}\n")

    for y in range(H):
        row = ""
        for x in range(W):
            dx = x - W // 2 + 0.5
            dy = y - H // 2 + 0.5
            inside = dx*dx + dy*dy <= R*R
            row += "1 " if inside else "0 "
        f.write(row.strip() + "\n")
