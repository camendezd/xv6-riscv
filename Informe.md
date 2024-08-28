Informe Sistemas operativos

Empezamos con el visualstudio, creando un repositorio, en este repositorio clonamos el ya clonado en git de xv6 desde el profesor, para luego crear
 otra carpeta que contenga los toolchain, en esta carpeta seguimos los pasos para la configuración y compilación 
del toolchain según esta guía; https://bernardnongpoh.github.io/posts/riscv#create-a-directory-in-opt
Al terminar el toolchain y compilarlo lo cual trajo problemas de versiones y path, se logro arreglarlo junto a nuestro mejor amigo chatgp.
Luego realizamos el comando de make qemu la cual inicia pero en segundo plano ya que el interfaz no se mostraba en la terminal, luego de investigar
nos damos cuenta de que se necesitaba descargar una versión actualizada del programa puesto que en varios foros se menciona que la causa del problema
puede deberse a utilizar una versión desactualizada de qemu. En la nueva carpeta de qemu clonamos el directorio https://gitlab.com/qemu-project/qemu.git
Luego de clonar este repositorio, iniciamos la siguiente secuencia de comandos; cd qemu, git checkout v7.2.0, 
./configure --target-list=riscv64-softmmu,riscv64-linux-user, make -j$(nproc), sudo make install, qemu-system-riscv64 --version finalmente para
confirmar la version para por ultimo comprobar que xv6 funciona con los comandos echo hola mundo y readme comando los cuales estan en 
la imagen ss.jpeg de la carpeta.

