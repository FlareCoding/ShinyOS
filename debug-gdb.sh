gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf" -ex "b _kmain" -ex "source gdb-layout.gdb" -ex "continue"
