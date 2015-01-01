
#include "ktypes.hpp"
#include "kio.hpp"
#include "dev/vgatext.hpp"
#include "dev/ps2.hpp"

struct mmentry {
	uint64_t start;
	uint64_t size;
	uint32_t type;
	uint32_t exattrib;
};

extern "C" {

void* malloc(size_t v) {
	if(v < 1) {
		return (void*)0x1000;
	}
	return nullptr;
}
void free(void *f) {
	if(f == ((void*)0x1000) ) {
		*((uint32_t*)f) = 7;
	}
}
void abort() {
	_ix_totalhalt();
}
size_t strlen(char *p) {
	size_t r = 0;
	while(*p != 0) r++;
	return r;
}

void _kernel_main() {
	uint32_t k = 0;
	uint32_t lim = *((uint16_t*)0x500);
	VGAText &vga = VGAText::dev;
	vga.putstr("Ixivix hello! ");
	hw::PS2 &kbd = hw::PS2::dev;
	kbd.init();
	while(true) {
		vga.setto(51,0);
		vga.puthex32(_ivix_int_n);
		vga.setto(51,1);
		vga.puthex32(k);
		vga.setto(45,21);
		vga.puthex32(kbd.err_n);
		vga.setto(55,20);
		vga.puthex32(cast<uint32_t>(kbd.port[0].status));
		vga.setto(65,20);
		vga.puthex32(cast<uint32_t>(kbd.port[1].status));
		vga.setto(55,21);
		vga.puthex32(cast<uint32_t>(kbd.port[0].type));
		vga.setto(65,21);
		vga.puthex32(cast<uint32_t>(kbd.port[1].type));
		vga.setto(55,22);
		vga.puthex8(kbd.icode[0]);
		vga.setto(58,22);
		vga.puthex8(kbd.istatus[0]);
		vga.setto(61,22);
		vga.puthex8(kbd.icode[1]);
		vga.setto(64,22);
		vga.puthex8(kbd.istatus[1]);
		vga.setto(55,23);
		vga.puthex32(kbd.interupted);
		if(kbd.waiting()) {
			kbd.handle();
			k++;
		}
		_ix_halt();
	}
	mmentry *mo = (mmentry*)0x800;
	for(uint32_t i = 0; i < lim; i++) {
		if(mo->type != 1) {
			mo->type = 2;
		}
		mo++;
	}
	_ix_totalhalt();
}

} // extern C
