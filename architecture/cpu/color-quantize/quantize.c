// #define RED0 0x00
// #define RED1 0x20
// #define RED2 0x40
// #define RED3 0x60
// #define RED4 0x80
// #define RED5 0xa0
// #define RED6 0xc0
// #define RED7 0xe0
// #define GREEN0 0x00
// #define GREEN1 0x04
// #define GREEN2 0x08
// #define GREEN3 0x0c
// #define GREEN4 0x10
// #define GREEN5 0x14
// #define GREEN6 0x18
// #define GREEN7 0x1c
// #define BLUE0 0x00
// #define BLUE1 0x01
// #define BLUE2 0x02
// #define BLUE3 0x03

// unsigned char quantize(unsigned char red, unsigned char green,
//                        unsigned char blue) {
//   unsigned char out = 0;
//   // 0x20 difference (32 in decimal?)
//   // can we use modulo to normalize the red value?
//   // (red // 32) * 32
//   if (red < 0x20) // 32
//     out += RED0; // 0x00 or 0
//   else if (red < 0x40) // 64
//     out += RED1; // 0x20 or 32
//   else if (red < 0x60)
//     out += RED2; // 0x40 or 64
//   else if (red < 0x80)
//     out += RED3; // 0x60
//   else if (red < 0xa0)
//     out += RED4; // 0x80
//   else if (red < 0xc0)
//     out += RED5; // 0xa0
//   else if (red < 0xe0)
//     out += RED6; // 0xc0
//   else
//     out += RED7; // 0xe0

//   // normalized green values go up by 4
//   if (green < 0x20)
//     out += GREEN0; // 0x00 or 0
//   else if (green < 0x40)
//     out += GREEN1; // 0x04 or 4
//   else if (green < 0x60)
//     out += GREEN2; // 0x08 or 8
//   else if (green < 0x80)
//     out += GREEN3; // 0x0c or 12
//   else if (green < 0xa0)
//     out += GREEN4; // 0x10 or 16
//   else if (green < 0xc0)
//     out += GREEN5; // 0x14 or 20
//   else if (green < 0xe0)
//     out += GREEN6; // 0x18 or 24
//   else
//     out += GREEN7; // 0x1c or 28

//   // normalized blue values go up by 1
//   if (blue < 0x40)
//     out += BLUE0; // 0x00
//   else if (blue < 0x80)
//     out += BLUE1; // 0x01
//   else if (blue < 0xc0)
//     out += BLUE2; // 0x02
//   else
//     out += BLUE3; // 0x03

//   return out;
// }

/*
 
0xe0
1110 0000
0010 0000 &
0010 0000

*/
unsigned char quantize(unsigned char red, unsigned char green,
  unsigned char blue) {
  unsigned char out = 0;

  out += (red / 0x20) * 0x20; // red & 0xe0
  out += (green / 0x20) * 0x04;
  out += blue / 0x40;

  return out;
}

