# Write the u-boot to the flash

* Use Blackfin BlueBoot Flash Writer
* Write u-boot-2011.09-2012R1-RC4_ucb-bf512.ldr.

# Transmit uClinux image and write it into the flash

* Use u-boot via UART. The baudrate is 57600.
* Transmit uImage-ucb-bf512_20121215.gz.initramfs with Kermit.

```
  bfin> loadb
  ## Ready for binary (kermit) download to 0x01000000 at 57600 bps...
  ## Total Size      = 0x003d47fe = 4016126 Bytes
  ## Start Addr      = 0x01000000
  bfin> sf probe 2
  SF: Detected M25P64 with page size 64 KiB, total 8 MiB
  bfin> sf write ${loadaddr} 0x80000 ${filesize}
```

