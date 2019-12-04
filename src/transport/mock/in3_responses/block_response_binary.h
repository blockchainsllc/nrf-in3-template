#ifndef MOCK_BLOCK_RESPONSE
#define MOCK_BLOCK_RESPONSE

//curl https://in3.stage.slock.it/mainnet/nd-3 -X POST -H "Content-Type:application/json" -d "{\"id\":1,\"jsonrpc\":\"2.0\",\"method\":\"eth_getBlockByNumber\",\"params\":[\"0x6a5c56\",true],\"in3\":{\"version\":\"2.1.0\",\"chainId\":\"0x1\",\"useFullProof\":true,\"verification\":\"proof\",\"useBinary\":true}}" -o block_response_binary.bin
unsigned char mock_block_response[] = {
  0xdc, 0x50, 0x64, 0xb6, 0xf9, 0x23, 0x32, 0x2e, 0x30, 0x00, 0xb6, 0x89,
  0x76, 0x3d, 0x15, 0x14, 0x52, 0xbc, 0x44, 0xd5, 0x37, 0x83, 0x09, 0xee,
  0x2a, 0xbf, 0x15, 0x39, 0xbf, 0x71, 0xde, 0x1b, 0x7d, 0x7b, 0xe3, 0xb5,
  0x75, 0x13, 0x07, 0x08, 0xb9, 0x50, 0xce, 0xca, 0x42, 0x54, 0x5d, 0x4a,
  0x0c, 0x6e, 0x61, 0x6e, 0x6f, 0x70, 0x6f, 0x6f, 0x6c, 0x2e, 0x6f, 0x72,
  0x67, 0x8a, 0x20, 0xbe, 0x7a, 0x12, 0x1d, 0x68, 0xd2, 0xbd, 0xe9, 0xae,
  0x4d, 0x92, 0x1c, 0x20, 0x4c, 0x45, 0xaa, 0xaf, 0x98, 0x3d, 0xe4, 0xbd,
  0x6c, 0xd8, 0x1f, 0xb0, 0xa6, 0x3f, 0x93, 0xd1, 0xee, 0xd1, 0x48, 0x24,
  0x2e, 0x1a, 0x08, 0xfe, 0x47, 0x7d, 0x48, 0x03, 0xd9, 0x18, 0x13, 0x72,
  0xcf, 0xd4, 0x1d, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x20, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0x7d, 0x83,
  0x29, 0x4e, 0x1c, 0x20, 0x40, 0x5c, 0x9e, 0x88, 0x1c, 0xc7, 0xbf, 0x61,
  0x34, 0x41, 0x72, 0x37, 0xc4, 0x5e, 0x0d, 0x33, 0x59, 0x52, 0x23, 0xfd,
  0x03, 0x53, 0x24, 0x9d, 0x67, 0xa6, 0x26, 0x2c, 0xef, 0xf0, 0x8c, 0xb0,
  0x46, 0x69, 0x08, 0x26, 0xc1, 0xba, 0x60, 0x0a, 0x01, 0x3d, 0xe1, 0xf3,
  0x03, 0xbe, 0x6a, 0x5c, 0x56, 0xab, 0xae, 0x1c, 0x20, 0x7e, 0x9d, 0xa3,
  0x4b, 0x60, 0xcc, 0x23, 0x21, 0x88, 0x20, 0x03, 0xcd, 0xca, 0x2c, 0x73,
  0x9e, 0x6d, 0xe3, 0x67, 0x42, 0xed, 0xb9, 0x24, 0xd3, 0x3d, 0x0c, 0xf2,
  0xfe, 0x86, 0x43, 0x9b, 0x6f, 0xd8, 0xa9, 0x1c, 0x20, 0xa0, 0xe1, 0x72,
  0x96, 0xe0, 0xf8, 0xba, 0xe1, 0xab, 0x29, 0xe3, 0x0d, 0xad, 0x46, 0xa9,
  0xc6, 0xf9, 0x32, 0xc3, 0xfd, 0xe2, 0x1d, 0xd4, 0xc1, 0x27, 0xe0, 0x06,
  0x6c, 0x67, 0x82, 0x35, 0x8e, 0x2a, 0xaa, 0x42, 0x1c, 0x21, 0xa0, 0x40,
  0x5c, 0x9e, 0x88, 0x1c, 0xc7, 0xbf, 0x61, 0x34, 0x41, 0x72, 0x37, 0xc4,
  0x5e, 0x0d, 0x33, 0x59, 0x52, 0x23, 0xfd, 0x03, 0x53, 0x24, 0x9d, 0x67,
  0xa6, 0x26, 0x2c, 0xef, 0xf0, 0x8c, 0xb0, 0x09, 0x88, 0x26, 0xc1, 0xba,
  0x60, 0x0a, 0x01, 0x3d, 0xe1, 0x60, 0xeb, 0x1c, 0x20, 0x1d, 0xcc, 0x4d,
  0xe8, 0xde, 0xc7, 0x5d, 0x7a, 0xab, 0x85, 0xb5, 0x67, 0xb6, 0xcc, 0xd4,
  0x1a, 0xd3, 0x12, 0x45, 0x1b, 0x94, 0x8a, 0x74, 0x13, 0xf0, 0xa1, 0x42,
  0xfd, 0x40, 0xd4, 0x93, 0x47, 0x84, 0x85, 0xbd, 0x02, 0xf7, 0x1e, 0xd1,
  0x1c, 0x20, 0xb0, 0x98, 0x9d, 0x11, 0xf6, 0x74, 0x25, 0x09, 0x0e, 0x12,
  0xf5, 0x93, 0x38, 0xb7, 0x47, 0xc5, 0x4a, 0xe0, 0x0c, 0xc9, 0xe6, 0xb9,
  0x6a, 0xd8, 0xb0, 0x65, 0x7a, 0x99, 0x79, 0xdf, 0xde, 0x74, 0x4a, 0xee,
  0x04, 0x5c, 0x26, 0xa3, 0xb8, 0x22, 0x51, 0x0a, 0x01, 0xcb, 0x56, 0xff,
  0x84, 0x26, 0xc3, 0x6f, 0x71, 0x0f, 0x81, 0x17, 0x42, 0x74, 0x9a, 0x5b,
  0x88, 0x64, 0xca, 0xbe, 0x6a, 0x5c, 0x56, 0x24, 0x40, 0xa1, 0x08, 0x73,
  0xc0, 0xbb, 0x77, 0xc0, 0x04, 0x96, 0x83, 0xf0, 0xf5, 0xbd, 0xc3, 0x50,
  0x42, 0xb8, 0x05, 0x02, 0x54, 0x0b, 0xe4, 0x00, 0x4d, 0x92, 0x1c, 0x20,
  0x4e, 0x63, 0x30, 0x5a, 0x36, 0x13, 0x39, 0xaa, 0x64, 0x9d, 0xd5, 0x42,
  0x92, 0xe7, 0x2b, 0x6c, 0xe1, 0xb6, 0x7e, 0x3e, 0x25, 0xf0, 0xed, 0x08,
  0x3e, 0x0e, 0x3a, 0xba, 0x26, 0x7f, 0x62, 0x28, 0x8d, 0xf6, 0xbb, 0x46,
  0x69, 0xbe, 0xa6, 0x33, 0xf2, 0xcf, 0xd6, 0x1c, 0x40, 0x95, 0x70, 0x27,
  0xfd, 0x3b, 0x16, 0x95, 0xf5, 0xe5, 0xf4, 0x4a, 0x54, 0x08, 0x36, 0xdf,
  0x36, 0xb6, 0xe1, 0x7d, 0xa3, 0xa2, 0x16, 0xb2, 0x0d, 0x83, 0x6f, 0x3e,
  0xca, 0xb5, 0x93, 0x53, 0xe7, 0x14, 0x7d, 0x92, 0x69, 0xbe, 0x6c, 0xd5,
  0x04, 0x9b, 0x02, 0xd8, 0x2c, 0x54, 0xe4, 0x24, 0x6d, 0x85, 0x3b, 0xf7,
  0xb2, 0x46, 0x64, 0x5c, 0xce, 0xb2, 0x3b, 0x8e, 0xc2, 0x21, 0x9a, 0x26,
  0x55, 0x00, 0x72, 0x1c, 0x20, 0x10, 0xc1, 0xba, 0x35, 0x54, 0x05, 0xef,
  0xc3, 0xd7, 0x2b, 0xf7, 0xfa, 0x4c, 0x8a, 0x66, 0x99, 0x21, 0xd0, 0x2b,
  0x10, 0x6f, 0xc2, 0x1c, 0xb5, 0x99, 0x0a, 0x0c, 0x64, 0xa8, 0x01, 0x80,
  0xcf, 0xb0, 0xe4, 0x1c, 0x71, 0xf8, 0x6f, 0x83, 0xa6, 0x33, 0xf2, 0x85,
  0x02, 0x54, 0x0b, 0xe4, 0x00, 0x82, 0xc3, 0x50, 0x94, 0x52, 0x19, 0x46,
  0x7f, 0x05, 0x82, 0x68, 0x91, 0x77, 0xe1, 0xf6, 0xa8, 0x1a, 0xa8, 0x13,
  0x73, 0x52, 0xa7, 0x15, 0x24, 0x88, 0x02, 0xc7, 0x15, 0x03, 0x24, 0xf7,
  0x0c, 0xa0, 0x80, 0x26, 0xa0, 0x10, 0xc1, 0xba, 0x35, 0x54, 0x05, 0xef,
  0xc3, 0xd7, 0x2b, 0xf7, 0xfa, 0x4c, 0x8a, 0x66, 0x99, 0x21, 0xd0, 0x2b,
  0x10, 0x6f, 0xc2, 0x1c, 0xb5, 0x99, 0x0a, 0x0c, 0x64, 0xa8, 0x01, 0x80,
  0xcf, 0xa0, 0x08, 0x6b, 0x85, 0xfc, 0x0b, 0x50, 0x62, 0x80, 0xe2, 0xc3,
  0xde, 0x8d, 0x75, 0xc8, 0xdb, 0x50, 0xc9, 0xb6, 0x69, 0x7d, 0x41, 0x89,
  0xec, 0xc7, 0x33, 0x87, 0x4c, 0x38, 0x57, 0xf6, 0xc7, 0x36, 0x00, 0x73,
  0x1c, 0x20, 0x08, 0x6b, 0x85, 0xfc, 0x0b, 0x50, 0x62, 0x80, 0xe2, 0xc3,
  0xde, 0x8d, 0x75, 0xc8, 0xdb, 0x50, 0xc9, 0xb6, 0x69, 0x7d, 0x41, 0x89,
  0xec, 0xc7, 0x33, 0x87, 0x4c, 0x38, 0x57, 0xf6, 0xc7, 0x36, 0x8f, 0xcd,
  0xa1, 0x3a, 0x1b, 0x14, 0x52, 0x19, 0x46, 0x7f, 0x05, 0x82, 0x68, 0x91,
  0x77, 0xe1, 0xf6, 0xa8, 0x1a, 0xa8, 0x13, 0x73, 0x52, 0xa7, 0x15, 0x24,
  0x04, 0xba, 0xa0, 0x00, 0x76, 0xbc, 0x26, 0x4a, 0x6b, 0x08, 0x02, 0xc7,
  0x15, 0x03, 0x24, 0xf7, 0x0c, 0xa0, 0x74, 0x9a, 0x5b, 0x88, 0x64, 0xca,
  0xbe, 0x6a, 0x5c, 0x56, 0x24, 0x40, 0xc0, 0x08, 0x73, 0xc0, 0xbb, 0x77,
  0xc0, 0x04, 0x96, 0x14, 0x5d, 0xca, 0xa1, 0xd8, 0xd8, 0x13, 0x2e, 0x5b,
  0xf9, 0xcf, 0x12, 0xde, 0xcc, 0xfc, 0x0c, 0xec, 0xf2, 0x6a, 0x78, 0x0d,
  0xf0, 0xf5, 0xbd, 0x52, 0x08, 0x42, 0xb8, 0x05, 0x03, 0xb9, 0xac, 0xa0,
  0x00, 0x4d, 0x92, 0x1c, 0x20, 0xc7, 0xa2, 0xaf, 0x47, 0xda, 0xba, 0x6b,
  0x39, 0x4a, 0xe0, 0x0c, 0x42, 0x2c, 0x3f, 0xf3, 0x5a, 0x7b, 0xbc, 0x37,
  0x62, 0x21, 0x28, 0xd0, 0x35, 0xb7, 0x8f, 0x88, 0x75, 0x8e, 0xc2, 0x73,
  0xb0, 0x8d, 0xf6, 0xbb, 0x46, 0x69, 0xbe, 0x02, 0xb7, 0x55, 0xcf, 0xd6,
  0x1c, 0x40, 0x17, 0x3e, 0xe5, 0x0d, 0x7e, 0x95, 0x61, 0x60, 0xd7, 0xaa,
  0xd0, 0xc9, 0x01, 0xd3, 0xf3, 0x9b, 0x78, 0x1c, 0x4f, 0xb3, 0x5d, 0x7f,
  0xca, 0x8b, 0xa6, 0x5c, 0xa5, 0x7b, 0x7e, 0xb8, 0x6c, 0x1b, 0x9a, 0x8d,
  0x82, 0x00, 0x67, 0xa9, 0x88, 0x20, 0xc2, 0xc0, 0x68, 0x32, 0x1b, 0x5f,
  0x43, 0xa5, 0x64, 0x7e, 0x9a, 0x4f, 0xf9, 0x34, 0x63, 0x66, 0x6d, 0xe4,
  0x71, 0x13, 0xc7, 0xb9, 0xb2, 0xbd, 0x00, 0x72, 0x1c, 0x20, 0xf8, 0x3c,
  0xae, 0x4b, 0x3e, 0xb5, 0xe7, 0xdd, 0xb6, 0xcf, 0x6e, 0x63, 0x03, 0x27,
  0xa3, 0x96, 0x1a, 0xe6, 0xb7, 0xb1, 0x4b, 0xea, 0x61, 0x70, 0x57, 0xc2,
  0x76, 0x88, 0xc3, 0x56, 0x89, 0xcd, 0xb0, 0xe4, 0x1c, 0x71, 0xf8, 0x6f,
  0x83, 0x02, 0xb7, 0x55, 0x85, 0x03, 0xb9, 0xac, 0xa0, 0x00, 0x82, 0x52,
  0x08, 0x94, 0xbe, 0x4e, 0x8d, 0x6d, 0xc0, 0xc0, 0xfd, 0xda, 0x60, 0xc7,
  0x4f, 0xfe, 0xa1, 0xeb, 0x35, 0xc2, 0x18, 0xa8, 0xd2, 0x64, 0x88, 0x1f,
  0x16, 0x14, 0x0a, 0x80, 0x69, 0x18, 0x00, 0x80, 0x1b, 0xa0, 0xf8, 0x3c,
  0xae, 0x4b, 0x3e, 0xb5, 0xe7, 0xdd, 0xb6, 0xcf, 0x6e, 0x63, 0x03, 0x27,
  0xa3, 0x96, 0x1a, 0xe6, 0xb7, 0xb1, 0x4b, 0xea, 0x61, 0x70, 0x57, 0xc2,
  0x76, 0x88, 0xc3, 0x56, 0x89, 0xcd, 0xa0, 0x49, 0x23, 0xb7, 0x04, 0x5b,
  0xd0, 0x0a, 0xbd, 0x79, 0xb4, 0x89, 0x98, 0x3e, 0x37, 0x98, 0xff, 0x33,
  0xd3, 0xf5, 0xd4, 0x1b, 0x10, 0x3b, 0xbb, 0x98, 0xd6, 0xb2, 0x68, 0x39,
  0xb3, 0xea, 0x93, 0x00, 0x73, 0x1c, 0x20, 0x49, 0x23, 0xb7, 0x04, 0x5b,
  0xd0, 0x0a, 0xbd, 0x79, 0xb4, 0x89, 0x98, 0x3e, 0x37, 0x98, 0xff, 0x33,
  0xd3, 0xf5, 0xd4, 0x1b, 0x10, 0x3b, 0xbb, 0x98, 0xd6, 0xb2, 0x68, 0x39,
  0xb3, 0xea, 0x93, 0x8f, 0xcd, 0xa0, 0x3a, 0x1b, 0x14, 0xbe, 0x4e, 0x8d,
  0x6d, 0xc0, 0xc0, 0xfd, 0xda, 0x60, 0xc7, 0x4f, 0xfe, 0xa1, 0xeb, 0x35,
  0xc2, 0x18, 0xa8, 0xd2, 0x64, 0x04, 0xba, 0xa1, 0x00, 0x76, 0xbb, 0x4a,
  0x6b, 0x08, 0x1f, 0x16, 0x14, 0x0a, 0x80, 0x69, 0x18, 0x00, 0xdf, 0xb1,
  0x1c, 0x20, 0xd1, 0x7b, 0x0e, 0x67, 0x6e, 0x56, 0xfe, 0xe2, 0x89, 0xc7,
  0xd9, 0xf3, 0x13, 0x43, 0x05, 0xeb, 0x1e, 0x98, 0xf3, 0xc7, 0x53, 0xd2,
  0x91, 0xdb, 0x85, 0x71, 0x8b, 0xcd, 0x2f, 0x40, 0xc8, 0x6e, 0xf9, 0x82,
  0x40, 0x34, 0x8d, 0xa1, 0x77, 0x34, 0x67, 0x4e, 0xe4, 0x62, 0x42, 0x18,
  0x2a, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x50, 0x72, 0x6f, 0x6f, 0x66, 0x00,
  0xb2, 0x97, 0x40, 0x87, 0xa0, 0xbe, 0x8a, 0x12, 0x3e, 0x6b, 0x72, 0xa0,
  0x45, 0x08, 0xbe, 0x65, 0x02, 0x93, 0x1c, 0xae, 0xbc, 0x27, 0xe4, 0xd4,
  0xbc, 0x22, 0x08, 0x22, 0xa1
};
unsigned int mock_block_response_len = 1517;
#endif
