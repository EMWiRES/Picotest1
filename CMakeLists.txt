add_executable(blinkingEye
        blinkingEye.c
	uncannyEyes.c
	timing.c
	SSD1351.c
        ST7735.c
	ILI9163C.c
	ILI9225.c
	spi.c
	led.c
	digitalIO.c
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(blinkingEye pico_stdlib hardware_spi)

# create map/bin/hex file etc.
pico_add_extra_outputs(blinkingEye)

# add url via pico_set_program_url
# example_auto_set_url(blinkingEye)
