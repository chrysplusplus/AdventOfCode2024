from pathlib import Path

cpp_files: list[Path] = sorted(Path(".").glob("*.cpp"))
readme_buffer: list[str] = []

for filename in cpp_files:
    with open(filename, "r") as file:
        # header comment lines are trimmed of first three characters
        line = file.readline().strip("\n")
        while line[1] == '*':
            readme_buffer.append(line[3:])
            line = file.readline().strip("\n")
            if len(line) == 0:
                break

with open("README.md", "w") as readme_file:
    for line in readme_buffer:
        print(line, file = readme_file)

