Pasos para compilar xv6:
Iniciar Ubuntu
git init para crear un repositorio en git
conectar mi cuenta usando 
# Configurar nombre de usuario
git config --global user.name "Tu Nombre"
# Configurar email
git config --global user.email "tu@email.com"

Hacer una actualización de dependencias:
sudo apt update
sudo apt install build-essential

Clonar el repositorio al que le hice fork:
git clone https://github.com/franciscarojasip/xv6-riscv.git

clonar riscv toolchain:
git clone https://github.com/riscv/riscv-gnu-toolchain

hacer los sudo apt-get para tener las dependencias necesarias del toolchain:
sudo apt-get install autoconf automake bison flex gawk gcc g++ libmpc-dev libmpfr-dev libgmp-dev texinfo

compilar el toolchain:
cd riscv-gnu-toolchain
sudo ./configure --prefix=/opt/riscv
sudo make newlib

añadir el PATH para poder usar el toolchain cuando compile en la terminal:
export PATH=/opt/riscv/bin:$PATH

clonar qemu:
git clone https://github.com/qemu/qemu.git

instalar dependencias:
sudo apt-get install build-essential libtool libglib2.0-dev
(tuve q instalar python3-venv, tomli y ninja eventualmente)

Ejecutar qemu:
cd qemu
./configure --target-list=riscv64-softmmu
make
sudo make install

ir a la carpeta de xv6-riscv 
make

Compilar sistema op:
make qemu :)

PARA SALIR DE QEMU: ctrl+A y X

Problemas que tuve en la tarea: tener que instalar dependencias que iban apareciendo como binutils y textinfo
Al final, tuve que usar este comando para instalar todas las dependencias que podría necesitar:
sudo apt-get install build-essential texinfo gawk bison flex libgmp-dev libmpfr-dev libmpc-dev
