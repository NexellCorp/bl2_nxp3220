include config.mak

LDFLAGS		=	-Bstatic							\
			-Wl,-Map=$(DIR_TARGETOUTPUT)/$(TARGET_NAME).map,--cref		\
			-T$(LDS_NAME).lds						\
			-Wl,--start-group						\
			-Lsrc/$(DIR_OBJOUTPUT)						\
			-Wl,--end-group							\
			-Wl,--build-id=none						\
			-nostdlib							\
			-Lsrc/lib -lddr


SYS_OBJS	+=	startup.o libnx.o libarmv7.o libplat.o armv7_pmu.o delay.o pll.o cmu.o clock.o serial.o printf.o	\
			dctrl.o dphy.o ${MEMTYPE}_sdram.o efuse.o memory.o pmu.o gpio.o i2c_gpio.o asv.o pmic.o sm5011.o 	\
			board_${BOARD}.o smc_entry.o tz.o plat_load.o build_info.o main.o


ifeq ($(MEMTEST), y)
SYS_OBJS	+=	memtester.o
endif


SYS_OBJS_LIST	+=	$(addprefix $(DIR_OBJOUTPUT)/,$(SYS_OBJS))

SYS_INCLUDES	+=	-I src/						\
			-I src/lib					\
			-I src/common					\
			-I src/include					\
			-I src/include/configs				\
			-I src/drivers					\
			-I src/drivers/clock				\
			-I src/drivers/serial				\
			-I src/drivers/pmic				\
			-I src/drivers/memory				\
			-I src/drivers/memory/ddr3			\
			-I src/drivers/memory/ddr4			\
			-I src/tests					\
			-I src/board					\
			-I src/services

###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/%.s
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(ASFLAG) $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################

###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/lib/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/lib/%.s
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(ASFLAG) $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################

##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/drivers/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/drivers/clock/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/drivers/serial/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/drivers/pmic/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################

##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/drivers/memory/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/drivers/memory/ddr3/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/drivers/memory/ddr4/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/drivers/memory/%.s
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(ASFLAG) $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################


##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/board/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################

##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/services/%.s
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(ASFLAG) $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################

##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/tests/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################

all: mkobjdir $(SYS_OBJS_LIST) link bin gen

link:
	@echo [link.... $(DIR_TARGETOUTPUT)/$(TARGET_NAME).elf]
	$(Q)$(CC) $(SYS_OBJS_LIST) $(LDFLAGS) -o $(DIR_TARGETOUTPUT)/$(TARGET_NAME).elf

bin:
	@echo [binary.... $(DIR_TARGETOUTPUT)/$(TARGET_NAME).bin]
	$(Q)$(MAKEBIN) -O binary $(DIR_TARGETOUTPUT)/$(TARGET_NAME).elf $(DIR_TARGETOUTPUT)/$(TARGET_NAME).bin

gen:
	./tools/rsa_sign -n reference-nsih/nsih_vtk_ddr3_800Mhz.txt -i out/bl2-${BOARD}.bin -b tools/bootkey -u tools/userkey -k bl2 -l 0xFFFF9000 -s 0xFFFF9000 -t

mkobjdir:
ifeq ($(OS),Windows_NT)
	@if not exist $(DIR_OBJOUTPUT)			\
		@$(MKDIR) $(DIR_OBJOUTPUT)
	@if not exist $(DIR_TARGETOUTPUT)		\
		@$(MKDIR) $(DIR_TARGETOUTPUT)
else
	@if	[ ! -e $(DIR_OBJOUTPUT) ]; then 	\
		$(MKDIR) $(DIR_OBJOUTPUT);		\
	fi;
	@if	[ ! -e $(DIR_TARGETOUTPUT) ]; then 	\
		$(MKDIR) $(DIR_TARGETOUTPUT);		\
	fi;
endif

clean:
ifeq ($(OS),Windows_NT)
	@if exist $(DIR_OBJOUTPUT)			\
		@$(RMDIR) $(DIR_OBJOUTPUT)
	@if exist $(DIR_TARGETOUTPUT)			\
		@$(RMDIR) $(DIR_TARGETOUTPUT)
else
	@if	[ -e $(DIR_OBJOUTPUT) ]; then 		\
		$(RMDIR) $(DIR_OBJOUTPUT);		\
	fi;
	@if	[ -e $(DIR_TARGETOUTPUT) ]; then 	\
		$(RMDIR) $(DIR_TARGETOUTPUT);		\
	fi;
endif

-include $(SYS_OBJS_LIST:.o=.d)
