nasm -f elf64 program.s -o program.o
nasm -f elf64 _start.s -o _start.o
x86_64-elf-ld program.o _start.o -o program
docker run --rm -v "$(pwd)":/work --platform linux/amd64 debian:bookworm /work/program
echo $?