import re

pdf_path = 'graphics_editor_prompt.pdf'

lines = [
    'Fake Project Prompt: C ASCII Graphics Editor',
    '',
    'Project Overview:',
    'Build a simple console-based C graphics editor that uses a 50x25 ASCII canvas and supports drawing lines, rectangles, triangles, and circles.',
    'The editor should allow adding, modifying, deleting, and listing objects, and then render the result in text form.',
    '',
    'Function Prompts:',
    '1. clear_canvas: Implement clear_canvas(canvas) to fill the entire canvas with the EMPTY character for a fresh drawing area.',
    '2. set_pixel: Implement set_pixel(canvas, x, y) to safely set a pixel only when coordinates are inside the grid bounds.',
    '3. display_canvas: Implement display_canvas(canvas) to print the current canvas rows and columns as ASCII art.',
    '4. draw_line: Implement draw_line(canvas, x1, y1, x2, y2) using Bresenham-style line drawing to connect two points.',
    '5. draw_rectangle: Implement draw_rectangle(canvas, x1, y1, x2, y2) to draw the outline of a rectangle using two opposite corners.',
    '6. draw_triangle: Implement draw_triangle(canvas, x1, y1, x2, y2, x3, y3) by drawing lines between three vertices.',
    '7. draw_circle: Implement draw_circle(canvas, xc, yc, radius) using a midpoint circle algorithm to draw a circle outline.',
    '8. draw_object: Implement draw_object(canvas, object) to dispatch drawing logic based on the object type.',
    '9. redraw_canvas: Implement redraw_canvas(canvas, objects, count) to clear the canvas and redraw every stored object.',
    '10. shape_name: Implement shape_name(type) to return a human-readable name for each shape enum value.',
    '11. list_objects: Implement list_objects(objects, count) to print stored object details and their coordinates.',
    '12. read_shape_type: Implement read_shape_type(type) to read the object type selection from the user.',
    '13. read_object: Implement read_object(object) to read shape-specific parameters from the user based on the chosen type.',
    '14. add_object: Implement add_object(objects, count) to append a new object to the object list after validating input.',
    '15. delete_object: Implement delete_object(objects, count) to remove an object by index and shift the remaining list.',
    '16. modify_object: Implement modify_object(objects, count) to update an existing object entry with new user input.',
    '17. print_menu: Implement print_menu() to show the main command menu options to the user.',
    '18. main: Implement main() to run the command loop, handle user choices, and maintain the canvas and objects.',
    '',
    'Data Structures:',
    '- ShapeType enum: Define LINE, RECTANGLE, TRIANGLE, CIRCLE values.',
    '- Object struct: Store object type, coordinates, and radius for circles.',
    '',
    'User Workflow:',
    '1. Start with an empty canvas.',
    '2. Add objects by type and coordinates.',
    '3. Modify or delete objects as needed.',
    '4. Display the canvas to view the current drawing.',
    '5. List objects for reference.',
]

# PDF text escaping for parentheses and backslashes.
def escape_pdf_text(text: str) -> str:
    return text.replace('\\', '\\\\').replace('(', '\\(').replace(')', '\\)')

stream_lines = ['BT', '/F1 12 Tf', '50 760 Td']
current_y = 760
line_height = 14
for line in lines:
    escaped_line = escape_pdf_text(line)
    stream_lines.append(f'({escaped_line}) Tj')
    current_y -= line_height
    if line != lines[-1]:
        stream_lines.append(f'0 -{line_height} Td')

stream_data = '\n'.join(stream_lines).encode('latin-1')

objects = []
objects.append(b'1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n')
objects.append(b'2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n')
objects.append(b'3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [0 0 612 792] /Contents 4 0 R /Resources << /Font << /F1 5 0 R >> >> >>\nendobj\n')
objects.append(b'4 0 obj\n<< /Length %d >>\nstream\n' % len(stream_data) + stream_data + b'\nendstream\nendobj\n')
objects.append(b'5 0 obj\n<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica >>\nendobj\n')

xref_start = 0
pdf = bytearray()
pdf.extend(b'%PDF-1.4\n%\xE2\xE3\xCF\xD3\n')

offsets = [0]
for obj in objects:
    offsets.append(len(pdf))
    pdf.extend(obj)

xref_start = len(pdf)
pdf.extend(b'xref\n0 %d\n' % (len(objects) + 1))
pdf.extend(b'0000000000 65535 f \n')
for offset in offsets[1:]:
    pdf.extend(f'{offset:010d} 00000 n \n'.encode('latin-1'))

pdf.extend(b'trailer\n<< /Size %d /Root 1 0 R >>\nstartxref\n%d\n%%%%EOF\n' % (len(objects) + 1, xref_start))

with open(pdf_path, 'wb') as f:
    f.write(pdf)

print(f'Created {pdf_path}')
