final: bootsect.o
	ld86 -0 -s -o bootsect bootsect.o
	dd bs=32 if=bootsect of=/dev/fd0 skip=1
	echo "output to /dev/fd0 successsfully"
bootsect.o: bootsect.s
	as86 -0 -a -o bootsect.o bootsect.s

clean:
	rm -f bootsect.o bootsect
