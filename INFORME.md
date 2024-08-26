## Informe Tarea 0

Este informe detalla el procedimiento que utilicé para instalar y compilar el sistema operativo xv6 en mi computador.

### Prerrequisitos
Antes que nada, decidi utilizar una partición utilizando el sistema operativo Ubuntu con su versión 22.04.3.
Luego de iniciar Ubuntu, habiéndolo instalado desde Microsoft Store y con WSL,
comencé en la terminal utilizando git init para crear un repositorio en git
conectar mi cuenta usando <br>
> git config --global user.name "Tu Nombre" <br>
> git config --global user.email "tu@email.com"

Antes de clonar nada, es necesario hacer una actualización de dependencias de la terminal: <br>
#sudo apt update <br>
#sudo apt install build-essential

### Instalación y Ejecución xv6 y Toolchain
Primero, se debió clonar el repositorio de xv6 original, al cual le hice fork previamente:<br>
>git clone https://github.com/franciscarojasip/xv6-riscv.git

Después, hacer lo mismo pero con el toolchain necesario para ejecutar el sistema:<br>
>git clone https://github.com/riscv/riscv-gnu-toolchain

A la vez, hay que instalar las dependencias necesarias del toolchain:<br>
>sudo apt-get install autoconf automake bison flex gawk gcc g++ libmpc-dev libmpfr-dev libgmp-dev texinfo

Por último, hay que compilar el toolchain: <br>
>cd riscv-gnu-toolchain <br>
>sudo ./configure --prefix=/opt/riscv <br>
>sudo make newlib

Después, añadir el PATH para poder usar el toolchain cuando compile en la terminal: <br>
>export PATH=/opt/riscv/bin:$PATH

### Instalación y Ejecución QEMU:
Ahora, se hace lo mismo, pero con el emulador QEMU: <br>
1. clonar qemu desde Github: <br>
>git clone https://github.com/qemu/qemu.git

2. Instalar dependencias necesarias para compilar qemu correctamente: <br>
>sudo apt-get install build-essential libtool libglib2.0-dev <br>
Eventualmente, tuve que instalar las dependencias python3-venv, tomli y ninja, que faltaban en mi terminal.

3. Para ejecutar qemu: <br>
>cd qemu <br>
>./configure --target-list=riscv64-softmmu <br>
>make <br>
>sudo make install <br>

Luego, hay que ir a la carpeta de xv6-riscv y ejecutar _make_

Finalmente, para compilar el sistema operativo: <br>
>make qemu

Cada vez que termine de utilizar el sistema, se sale de QEMU utilizando _ctrl+A y X_

#### Precaución
Debo asegurarme,cada vez que compile el sistema operativo, que el PATH sea el adecuado,
por lo que en cada sesión nueva de terminal, antes de make qemu, se 
ejecuta export PATH=/opt/riscv/bin:$PATH

### Problemas y cómo se solucionaron
Tuve muchos problemas para compilar este sistema, principalmente por el tener que instalar dependencias que iba necesitando, como binutils y textinfo.
Al final, tuve que usar este comando para instalar todas las dependencias que podría necesitar:
>sudo apt-get install build-essential texinfo gawk bison flex libgmp-dev libmpfr-dev libmpc-dev
