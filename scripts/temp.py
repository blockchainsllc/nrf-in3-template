def gimme_unique_lines(source_file_path, new_file_path):

    source_lines = []
    new_lines = []

    with open(source_file_path) as source_file:
        source_lines = source_file.readlines()

    with open(new_file_path) as new_file:
        new_lines = new_file.readlines()

    unique_lines = []

    for newline in new_lines:
        if "#define" in newline:
            if newline not in source_lines:
                unique_lines.append(newline)

    print(unique_lines)
    print("\n")
    print("".join(unique_lines))


gimme_unique_lines("./config/sdk_config.h", "./include/sdk_config.h")
