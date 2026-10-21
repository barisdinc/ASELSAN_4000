# Regenerate the C translation and run all equivalence tests.
#   make            -> lift + build + test
#   make HEX=../YA4000D.HEX   (another variant)
HEX     ?= ../YA4000S.HEX
CC      ?= clang
PY      ?= python3 -I
TMP     ?= /tmp/aselsan_test
LIFTED  := c_lifted
CLEAN   := c_clean
TESTDEF := '-DREG8(a)=(test_regs[(a)])' '-DREG16(a)=(*(volatile uint16_t *)&test_regs[(a)])' -include test/regs.h

JSC     := /System/Library/Frameworks/JavaScriptCore.framework/Versions/Current/Helpers/jsc
JSRUN   := $(shell command -v node >/dev/null 2>&1 && echo node || echo "$(JSC)")
JSSEP   := $(if $(findstring node,$(JSRUN)),,--)

all: lift tables build test sim test-sim test-a77 test-ozel

lift:
	$(PY) tools/lift.py $(HEX) $(LIFTED)

tables:
	$(PY) tools/gen_tables.py $(HEX) $(CLEAN)/tone_tables.c

build:
	cd $(LIFTED) && $(CC) -std=c99 -Wall -Wno-unused-label -Wno-unused-variable -c *.c && rm -f *.o
	cd $(CLEAN) && $(CC) -std=c99 -Wall -Wextra -fsyntax-only i2c.c eeprom.c synth.c ctcss.c tones.c tone_tables.c

test: test-runtime test-boot test-funcs test-clean

# web simulator: embed the HEX files, then run its regression test
# example of a modified firmware: power-on messages (tools/acilis_mesaji.py)
../ozel_firmware/YA4000S_MERHABA.HEX: ../YA4000S.HEX tools/acilis_mesaji.py
	mkdir -p ../ozel_firmware
	$(PY) tools/acilis_mesaji.py ../YA4000S.HEX $@ "MERHABA" "DÜNYA" "YENİDEN" "ATÖLYE"

# assembler examples (tools/a77.py + tools/yama.py): ozel_firmware/ornekler/*.asm
ORNEKLER := ../ozel_firmware/YA4000S_SAYI.HEX ../ozel_firmware/YA4000S_HESAP.HEX ../ozel_firmware/YA4000S_OYUN.HEX ../ozel_firmware/YA4000S_FREKANS.HEX ../ozel_firmware/YA4000S_FREKANS_YATOLYE.HEX
ORNEK_SRC := ../ozel_firmware/ornekler/menu_altyapi.inc tools/a77.py tools/yama.py ../ozel_firmware/surum.json

../ozel_firmware/YA4000S_SAYI.HEX: ../YA4000S.HEX ../ozel_firmware/ornekler/sayi_ters.asm ../ozel_firmware/ornekler/sayi_ters.inc $(ORNEK_SRC)
	$(PY) tools/yama.py ../YA4000S.HEX $@ ../ozel_firmware/ornekler/sayi_ters.asm -l ../ozel_firmware/ornekler/sayi_ters.lst --surum ../ozel_firmware/surum.json

../ozel_firmware/YA4000S_HESAP.HEX: ../YA4000S.HEX ../ozel_firmware/ornekler/hesap.asm ../ozel_firmware/ornekler/hesap.inc ../ozel_firmware/ornekler/sayi_ters.inc $(ORNEK_SRC)
	$(PY) tools/yama.py ../YA4000S.HEX $@ ../ozel_firmware/ornekler/hesap.asm -l ../ozel_firmware/ornekler/hesap.lst --surum ../ozel_firmware/surum.json

../ozel_firmware/YA4000S_OYUN.HEX: ../YA4000S.HEX ../ozel_firmware/ornekler/oyun.asm ../ozel_firmware/ornekler/oyun.inc ../ozel_firmware/ornekler/hesap.inc ../ozel_firmware/ornekler/sayi_ters.inc $(ORNEK_SRC)
	$(PY) tools/yama.py ../YA4000S.HEX $@ ../ozel_firmware/ornekler/oyun.asm -l ../ozel_firmware/ornekler/oyun.lst --surum ../ozel_firmware/surum.json

../ozel_firmware/YA4000S_FREKANS.HEX: ../YA4000S.HEX ../ozel_firmware/ornekler/frekans.asm ../ozel_firmware/ornekler/frekans.inc ../ozel_firmware/ornekler/aprs.inc ../ozel_firmware/ornekler/aprs_rx.inc ../ozel_firmware/ornekler/secmeli_kaldir.inc ../ozel_firmware/ornekler/tarama_tks.inc ../ozel_firmware/ornekler/aprs_seri.inc ../ozel_firmware/ornekler/yazici_kaldir.inc ../ozel_firmware/ornekler/ozellik_kaldir.inc ../ozel_firmware/ornekler/dil_kaldir.inc ../ozel_firmware/ornekler/aprs_detay.inc ../ozel_firmware/ornekler/aprs_ayar.inc $(ORNEK_SRC)
	$(PY) tools/yama.py ../YA4000S.HEX $@ ../ozel_firmware/ornekler/frekans.asm -l ../ozel_firmware/ornekler/frekans.lst --surum ../ozel_firmware/surum.json

../ozel_firmware/YA4000S_FREKANS_YATOLYE.HEX: ../YA4000S.HEX ../ozel_firmware/ornekler/frekans_yatolye.asm ../ozel_firmware/ornekler/acilis_yatolye.inc ../ozel_firmware/ornekler/frekans.asm ../ozel_firmware/ornekler/frekans.inc ../ozel_firmware/ornekler/aprs.inc ../ozel_firmware/ornekler/aprs_rx.inc ../ozel_firmware/ornekler/secmeli_kaldir.inc ../ozel_firmware/ornekler/tarama_tks.inc ../ozel_firmware/ornekler/aprs_seri.inc ../ozel_firmware/ornekler/yazici_kaldir.inc ../ozel_firmware/ornekler/ozellik_kaldir.inc ../ozel_firmware/ornekler/dil_kaldir.inc ../ozel_firmware/ornekler/aprs_detay.inc ../ozel_firmware/ornekler/aprs_ayar.inc $(ORNEK_SRC)
	$(PY) tools/yama.py ../YA4000S.HEX $@ ../ozel_firmware/ornekler/frekans_yatolye.asm -l ../ozel_firmware/ornekler/frekans_yatolye.lst --surum ../ozel_firmware/surum.json

ozel: ../ozel_firmware/YA4000S_MERHABA.HEX $(ORNEKLER)

# a77 must assemble the disassembly back into the original bytes
test-a77:
	$(PY) tools/test_a77.py ../YA4000S.HEX YA4000S.asm
	$(PY) tools/test_a77.py ../YA4000E.HEX YA4000E.asm

# examples in the simulator (also flashes the image through the serial loader)
test-ozel: $(ORNEKLER)
	cd sim && $(JSRUN) test_ozel.js $(JSSEP) ../../ozel_firmware/YA4000S_SAYI.HEX ../../YA4000S.HEX NUMBER
	cd sim && $(JSRUN) test_ozel.js $(JSSEP) ../../ozel_firmware/YA4000S_HESAP.HEX ../../YA4000S.HEX NUMBER,CALC
	cd sim && $(JSRUN) test_ozel.js $(JSSEP) ../../ozel_firmware/YA4000S_OYUN.HEX ../../YA4000S.HEX NUMBER,CALC,GAME
	cd sim && $(JSRUN) test_ozel.js $(JSSEP) ../../ozel_firmware/YA4000S_FREKANS.HEX ../../YA4000S.HEX 'FREQ,APRS,APRS TR*,APRS DG-,APRS IG*'
	cd sim && $(JSRUN) test_ozel.js $(JSSEP) ../../ozel_firmware/YA4000S_FREKANS_YATOLYE.HEX ../../YA4000S.HEX 'FREQ,APRS,APRS TR*,APRS DG-,APRS IG*'

sim: ozel
	$(PY) sim/mkfirmware.py --surum ../ozel_firmware/surum.json --notlar ../ozel_firmware/degisiklikler.json \
	    ../YA4000S.HEX ../YA4000D.HEX ../YA4000E.HEX ../YA4000K.HEX \
	    ../YA4000P.HEX ../YA4000R.HEX ../YA4000V.HEX ../YA4000Y.HEX \
	    ../ozel_firmware/YA4000S_FREKANS_YATOLYE.HEX > sim/firmware.js
	$(PY) tools/surum_notlari.py ../ozel_firmware/degisiklikler.json ../ozel_firmware/surum.json > ../ozel_firmware/SURUMLER.md

# read-only serial client (yukle.html) against the simulated radio
test-yukle:
	cd sim && $(JSRUN) yukle_test.js $(JSSEP) ../../YA4000S.HEX ../../ozel_firmware/YA4000S_FREKANS_YATOLYE.HEX

# P4000 EEPROM yerleşimi ve .p40 kütükleri (p4000.js); gerçek döküm / kütük de verilebilir:
#   make test-p4000 P4000_DOSYA="~/eeprom.bin ~/P4000/aprs.p40"
# programlama sayfasının sayaç sunucusu (sunucu/sayac, Cloudflare Worker)
test-sayac:
	cd ../sunucu/sayac && $(if $(findstring node,$(JSRUN)),node test.mjs,$(JSC) -m test.mjs)

test-p4000:
	cd sim && $(JSRUN) test_p4000.js $(JSSEP) $(P4000_DOSYA)

test-sim:
	cd sim && $(JSRUN) selftest.js $(JSSEP) ../$(HEX)

test-runtime:
	cd $(LIFTED) && $(CC) -std=c99 -O1 -Wno-everything -o test/test_runtime test/test_runtime.c *.c && ./test/test_runtime

test-boot:
	mkdir -p $(TMP)
	cd $(LIFTED) && $(CC) -std=c99 -O1 -Wno-everything -o test/trace_main test/trace_main.c *.c
	for s in 1 2 3; do \
	  SEED=$$s $(LIFTED)/test/trace_main 500000 > $(TMP)/c$$s.txt; \
	  SEED=$$s $(PY) tools/emu7700.py $(HEX) 500000 $(TMP)/e$$s.txt > /dev/null; \
	  cmp $(TMP)/c$$s.txt $(TMP)/e$$s.txt && echo "boot trace seed $$s: identical"; \
	done

test-funcs:
	mkdir -p $(TMP)
	cd $(LIFTED) && $(CC) -std=c99 -O1 -Wno-everything -include test/tick.h '-DTICK()=tick_hook()' \
	    -o test/func_diff test/func_diff.c test/func_table.c *.c
	$(LIFTED)/test/func_diff 10 > $(TMP)/fc.txt
	$(PY) tools/emu7700.py --funcs $(HEX) $(LIFTED)/test/func_table.c 10 $(TMP)/fe.txt
	$(PY) tools/fdcompare.py $(TMP)/fc.txt $(TMP)/fe.txt

test-clean:
	cd $(CLEAN) && $(CC) -std=c99 -O1 -Wno-everything $(TESTDEF) -o test/test_ctcss test/test_ctcss.c \
	    ctcss.c tone_tables.c ../$(LIFTED)/[a-z]*.c -lm && ./test/test_ctcss | tail -1
	cd $(CLEAN) && $(CC) -std=c99 -O1 -Wno-everything $(TESTDEF) -o test/test_tones test/test_tones.c \
	    tones.c tone_tables.c ../$(LIFTED)/[a-z]*.c && ./test/test_tones
	cd $(CLEAN) && $(CC) -std=c99 -O1 -Wall $(TESTDEF) -o test/test_synth test/test_synth.c synth.c && ./test/test_synth

.PHONY: all lift tables build test test-runtime test-boot test-funcs test-clean sim test-sim ozel test-a77 test-ozel

