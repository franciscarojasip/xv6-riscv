Este informe detalla el procedimiento que utilicé para instalar y compilar el sistema operativo xv6 en mi computador.

Antes que nada, decidi utilizar una partición utilizando el sistema operativo Ubuntu con su versión 22.04.3.
Luego de iniciar Ubuntu, habiéndolo instalado desde Microsoft Store y con WSL,
comencé en la terminal utilizando git init para crear un repositorio en git
conectar mi cuenta usando 
#git config --global user.name "Tu Nombre"
#git config --global user.email "tu@email.com"

Antes de clonar nada, es necesario hacer una actualización de dependencias de la terminal:
#sudo apt update
#sudo apt install build-essential

Primero, se debió clonar el repositorio de xv6 original, al cual le hice fork previamente:
#git clone https://github.com/franciscarojasip/xv6-riscv.git

Después, hacer lo mismo pero con el toolchain necesario para ejecutar el sistema:
#git clone https://github.com/riscv/riscv-gnu-toolchain

A la vez, hay que instalar las dependencias necesarias del toolchain:
#sudo apt-get install autoconf automake bison flex gawk gcc g++ libmpc-dev libmpfr-dev libgmp-dev texinfo

Por último, hay que compilar el toolchain:
#cd riscv-gnu-toolchain
#sudo ./configure --prefix=/opt/riscv
#sudo make newlib

Añadir el PATH para poder usar el toolchain cuando compile en la terminal:
#export PATH=/opt/riscv/bin:$PATH

Ahora, se hace lo mismo, pero con el emulador QEMU:
1. clonar qemu desde Github:
#git clone https://github.com/qemu/qemu.git

2. Instalar dependencias necesarias para compilar qemu correctamente:
#sudo apt-get install build-essential libtool libglib2.0-dev
Eventualmente, tuve que instalar las dependencias python3-venv, tomli y ninja, que faltaban en mi terminal.

Para ejecutar qemu:
#cd qemu
#./configure --target-list=riscv64-softmmu
#make
#sudo make install

Luego, hay que ir a la carpeta de xv6-riscv y ejecutar make

Para compilar el sistema operativo:
#make qemu

Finalmente, para salir de qemu: ctrl+A y X

Debo asegurarme,cada vez que compile el sistema operativo, que el PATH sea el adecuado,
por lo que en cada sesión nueva de terminal, antes de make qemu, se 
ejecuta export PATH=/opt/riscv/bin:$PATH

Problemas que tuve en la tarea: Tener que instalar dependencias que iban apareciendo como binutils y textinfo
Al final, tuve que usar este comando para instalar todas las dependencias que podría necesitar:
sudo apt-get install build-essential texinfo gawk bison flex libgmp-dev libmpfr-dev libmpc-dev
