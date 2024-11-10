#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/GCPlus2.0_Bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/GCPlus2.0_Bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=main.c config.c nvm.c adc.c buttons.c si.c rumble.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.p1 ${OBJECTDIR}/config.p1 ${OBJECTDIR}/nvm.p1 ${OBJECTDIR}/adc.p1 ${OBJECTDIR}/buttons.p1 ${OBJECTDIR}/si.p1 ${OBJECTDIR}/rumble.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/main.p1.d ${OBJECTDIR}/config.p1.d ${OBJECTDIR}/nvm.p1.d ${OBJECTDIR}/adc.p1.d ${OBJECTDIR}/buttons.p1.d ${OBJECTDIR}/si.p1.d ${OBJECTDIR}/rumble.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.p1 ${OBJECTDIR}/config.p1 ${OBJECTDIR}/nvm.p1 ${OBJECTDIR}/adc.p1 ${OBJECTDIR}/buttons.p1 ${OBJECTDIR}/si.p1 ${OBJECTDIR}/rumble.p1

# Source Files
SOURCEFILES=main.c config.c nvm.c adc.c buttons.c si.c rumble.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/GCPlus2.0_Bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F25K42
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/config.p1: config.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/config.p1.d 
	@${RM} ${OBJECTDIR}/config.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/config.p1 config.c 
	@-${MV} ${OBJECTDIR}/config.d ${OBJECTDIR}/config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/nvm.p1: nvm.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/nvm.p1.d 
	@${RM} ${OBJECTDIR}/nvm.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/nvm.p1 nvm.c 
	@-${MV} ${OBJECTDIR}/nvm.d ${OBJECTDIR}/nvm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/nvm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/adc.p1: adc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc.p1.d 
	@${RM} ${OBJECTDIR}/adc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/adc.p1 adc.c 
	@-${MV} ${OBJECTDIR}/adc.d ${OBJECTDIR}/adc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/adc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/buttons.p1: buttons.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/buttons.p1.d 
	@${RM} ${OBJECTDIR}/buttons.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/buttons.p1 buttons.c 
	@-${MV} ${OBJECTDIR}/buttons.d ${OBJECTDIR}/buttons.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/buttons.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/si.p1: si.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/si.p1.d 
	@${RM} ${OBJECTDIR}/si.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/si.p1 si.c 
	@-${MV} ${OBJECTDIR}/si.d ${OBJECTDIR}/si.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/si.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/rumble.p1: rumble.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rumble.p1.d 
	@${RM} ${OBJECTDIR}/rumble.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/rumble.p1 rumble.c 
	@-${MV} ${OBJECTDIR}/rumble.d ${OBJECTDIR}/rumble.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/rumble.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/config.p1: config.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/config.p1.d 
	@${RM} ${OBJECTDIR}/config.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/config.p1 config.c 
	@-${MV} ${OBJECTDIR}/config.d ${OBJECTDIR}/config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/nvm.p1: nvm.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/nvm.p1.d 
	@${RM} ${OBJECTDIR}/nvm.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/nvm.p1 nvm.c 
	@-${MV} ${OBJECTDIR}/nvm.d ${OBJECTDIR}/nvm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/nvm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/adc.p1: adc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc.p1.d 
	@${RM} ${OBJECTDIR}/adc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/adc.p1 adc.c 
	@-${MV} ${OBJECTDIR}/adc.d ${OBJECTDIR}/adc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/adc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/buttons.p1: buttons.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/buttons.p1.d 
	@${RM} ${OBJECTDIR}/buttons.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/buttons.p1 buttons.c 
	@-${MV} ${OBJECTDIR}/buttons.d ${OBJECTDIR}/buttons.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/buttons.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/si.p1: si.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/si.p1.d 
	@${RM} ${OBJECTDIR}/si.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/si.p1 si.c 
	@-${MV} ${OBJECTDIR}/si.d ${OBJECTDIR}/si.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/si.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/rumble.p1: rumble.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rumble.p1.d 
	@${RM} ${OBJECTDIR}/rumble.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/rumble.p1 rumble.c 
	@-${MV} ${OBJECTDIR}/rumble.d ${OBJECTDIR}/rumble.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/rumble.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/GCPlus2.0_Bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/GCPlus2.0_Bootloader.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -mdebugger=pickit3  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/GCPlus2.0_Bootloader.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} ${DISTDIR}/GCPlus2.0_Bootloader.X.${IMAGE_TYPE}.hex 
	
else
${DISTDIR}/GCPlus2.0_Bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../GCPlus2.0.X/dist/default/production/GCPlus2.0.X.production.hex
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/GCPlus2.0_Bootloader.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1    -fshort-double -fshort-float -memi=wordwrite -mrom=0-1FFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -mc90lib -std=c90 -gdwarf-3 -mstack=compiled:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/GCPlus2.0_Bootloader.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
	@echo "Creating unified hex file"
	@"/Applications/microchip/mplabx/v6.15/MPLAB X IDE v6.15.app/Contents/Resources/mplab_ide/platform/../mplab_ide/modules/../../bin/hexmate" --edf="/Applications/microchip/mplabx/v6.15/MPLAB X IDE v6.15.app/Contents/Resources/mplab_ide/platform/../mplab_ide/modules/../../dat/en_msgs.txt" ${DISTDIR}/GCPlus2.0_Bootloader.X.${IMAGE_TYPE}.hex ../GCPlus2.0.X/dist/default/production/GCPlus2.0.X.production.hex -odist/${CND_CONF}/production/GCPlus2.0_Bootloader.X.production.unified.hex

endif


# Subprojects
.build-subprojects:
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
	cd ../GCPlus2.0.X && ${MAKE}  -f Makefile CONF=default TYPE_IMAGE=DEBUG_RUN
else
	cd ../GCPlus2.0.X && ${MAKE}  -f Makefile CONF=default
endif


# Subprojects
.clean-subprojects:
	cd ../GCPlus2.0.X && rm -rf "build/default" "dist/default"

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
