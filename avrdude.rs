
	programmer
	  id    = "AVR_TPI";
	  desc  = "AVR TPI programmer";
	  baudrate = 115200;
	  type  = "avr910";
	;

#------------------------------------------------------------
# ATtiny10
#------------------------------------------------------------

part
    id		= "attiny10";
    desc	= "ATtiny10";
		avr910_devcode = 0x10;
    signature	= 0x1e 0x90 0x03;
		timeout		= 900;

    memory "flash"
        size			= 1024;
        page_size	= 64;						# page_size is really 16 but consider this buffer size for uno_tpi
        blocksize	= 128;
				readsize	= 256;
    ;

    memory "signature"
        size		= 3;
			 read            = "0 0 1 1  0 0 0 0  0 0 0 0  0 0 0 0",
							           "0 0 0 0  0 0 a1 a0  o o o o  o o o o";
			min_write_delay = 9000;
			max_write_delay = 9000;
    ;

    memory "calibration"
        size		= 1;
			 read            = "0 0 1 1  1 0 0 0  0 0 0 0  0 0 0 0",
							           "0 0 0 0  0 0 0 0  o o o o  o o o o";
			min_write_delay = 9000;
			max_write_delay = 9000;
    ;

    memory "fuse"
 			 size            = 	1;
			 read            = 	"0 1 0 1  0 0 0 0  0 0 0 0  0 0 0 0",
							   					"x x x x  x x x x  o o o o  o o o o";

			 write           = 	"1 0 1 0  1 1 0 0  1 0 1 0  1 0 0 0",
							   					"x x x x  x x x x  i i i i  i i i i";
 			min_write_delay = 9000;
			max_write_delay = 9000;
    ;

    memory "lock"
        size		= 1;
        read            = "0 1 0 1  1 0 0 0   0 0 0 0  0 0 0 0",
                          "x x x x  x x x x   o o o o  o o o o";

        write           = "1 0 1 0  1 1 0 0   1 1 1 0  0 0 0 0",
                          "x x x x  x x x x   i i i i  i i i i";
			min_write_delay = 9000;
			max_write_delay = 9000;
    ;
;

