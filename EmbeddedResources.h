#pragma once

#include <cstddef>

// Embedded image resources
extern const unsigned char IDI_ICON_data[];
extern const size_t IDI_ICON_data_size;
extern const unsigned char IDB_UNMARKED_data[];
extern const size_t IDB_UNMARKED_data_size;
extern const unsigned char IDB_MARKED_data[];
extern const size_t IDB_MARKED_data_size;
extern const unsigned char IDB_COLD_data[];
extern const size_t IDB_COLD_data_size;
extern const unsigned char IDB_WARM_data[];
extern const size_t IDB_WARM_data_size;
extern const unsigned char IDB_HOT_data[];
extern const size_t IDB_HOT_data_size;
extern const unsigned char IDB_ALMOST_data[];
extern const size_t IDB_ALMOST_data_size;
extern const unsigned char IDB_BIGBUTTON_data[];
extern const size_t IDB_BIGBUTTON_data_size;
extern const unsigned char IDB_CORRECT_data[];
extern const size_t IDB_CORRECT_data_size;
extern const unsigned char IDB_UNUSED_data[];
extern const size_t IDB_UNUSED_data_size;
extern const unsigned char IDB_USED_data[];
extern const size_t IDB_USED_data_size;

// Embedded word list
extern const char* EMBEDDED_WORDS[];
extern const size_t EMBEDDED_WORDS_COUNT;

// Initialize all embedded resources in wxMemoryFSHandler
void InitializeEmbeddedResources();
