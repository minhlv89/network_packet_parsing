#include <iostream>
#include <fstream>

using namespace std;
int main(void) {
	streampos size;
	char * memblock;

	ifstream file("test.txt", ios::in | ios::binary | ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char[size];
		file.seekg(0, ios::beg);
		file.read(memblock, size);
		file.close();

		cout << "the entire file content is in memory";

		delete[] memblock;
	}
	else cout << "Unable to open file";
	return 0;
	/*
	while (1) {
		while (iface.existRcvData()) {
		uint8_t byte = iface.getByte();
		// Parse incoming bytes and detect packet
		if (parser.detectPkt(byte)) {
		// One, correct packet has been detected
		CompoTlm_t tlm = {};
		int err = parser.extractData(tlm);
		if (err == 0) {
		// Pass telemetry info to the upper layer
		system.tlm.update(tlm);
		}
		else {
		// Error handling, etc.
		}
		}
		}
		
		system.time.sleep_ms(100);
	}
	*/
}