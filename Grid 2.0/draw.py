from PIL import Image, ImageDraw

point_size = 1
scale = 100
w, h =1000, 1000
offsetx, offsety = 100, 100
x = []
y = []
fes = []
edges = []

def convert_x(x):
    return x * scale + offsetx

def convert_y(y):
    return h - y * scale - offsety

def draw_point(x, y):
    draw.ellipse((x-point_size, y-point_size, x+point_size, y+point_size), fill="black")

def draw_point_good(x, y):
    draw_point(convert_x(x), convert_y(y))

def draw_fe(i):
    draw.line((convert_x(x[fes[i][0]]), convert_y(y[fes[i][0]]), convert_x(x[fes[i][1]]), convert_y(y[fes[i][1]])), fill="black")
    draw.line((convert_x(x[fes[i][1]]), convert_y(y[fes[i][1]]), convert_x(x[fes[i][3]]), convert_y(y[fes[i][3]])), fill="black")
    draw.line((convert_x(x[fes[i][3]]), convert_y(y[fes[i][3]]), convert_x(x[fes[i][2]]), convert_y(y[fes[i][2]])), fill="black")
    draw.line((convert_x(x[fes[i][2]]), convert_y(y[fes[i][2]]), convert_x(x[fes[i][0]]), convert_y(y[fes[i][0]])), fill="black")

def draw_number(x, y, n):
    draw.text((convert_x(x) + 10, convert_y(y) - 10), str(n), fill="black")

def draw_coordinats(x, y):
    draw.text((convert_x(x) + 20, convert_y(y) - 20), str(x) + " " + str(y), fill="black")

def draw_edge_number(i):
    xx = (x[edges[i][0]] + x[edges[i][1]]) / 2
    xx = convert_x(xx)
    yy = (y[edges[i][0]] + y[edges[i][1]]) / 2
    yy = convert_y(yy)
    draw.text((xx + 5, yy - 10), str(i), fill="black")

f = open('nodes.txt')
for line in f:
    data = line.split(" ")
    x.append(float(data[0]))
    y.append(float(data[1]))

f = open('fes.txt')
for line in f:
    data = line.split(" ")
    l = []
    l.append(int(data[0]))
    l.append(int(data[1]))
    l.append(int(data[2]))
    l.append(int(data[3]))
    fes.append(l)

f = open('edges.txt')
for line in f:
    data = line.split(" ")
    l = []
    l.append(int(data[0]))
    l.append(int(data[1]))
    edges.append(l)

img = Image.new("RGB", (w, h))


draw = ImageDraw.Draw(img)
draw.rectangle((0,0,w,h), fill="white")

for i in range(0, len(x)):
    draw_point_good(x[i], y[i])
    # draw_number(x[i], y[i], i)
    # draw_coordinats(x[i], y[i])

for i in range(0, len(fes)):
    draw_fe(i)

for i in range(0, len(edges)):
    draw_edge_number(i)
# draw.text((500,500), "help me", fill="black")
img.show()