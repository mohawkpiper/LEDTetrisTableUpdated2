/* FastLED_RGBW
*
* Hack to enable SK6812 RGBW strips to work with FastLED.
*
* Original code by Jim Bumgardner (http://krazydad.com).
* Modified by David Madison (http://partsnotincluded.com).
* Added stuff by Greg Beckman
*
*/

#ifndef FastLED_RGBW_h
#define FastLED_RGBW_h

struct CRGBW {
	union {
		struct {
			union {
				uint8_t g;
				uint8_t green;
			};
			union {
				uint8_t r;
				uint8_t red;
			};
			union {
				uint8_t b;
				uint8_t blue;
			};
			union {
				uint8_t w;
				uint8_t white;
			};
		};
		uint8_t raw[4];
	};

	CRGBW() {}

  CRGBW(uint16_t rd, uint16_t grn, uint16_t blu) {
    //Calibrated values
    const float RC = 255;
    const float GC = 128;
    const float BC = 34;
    const float WC = 100;
        
    //Normalized red, green, and blue
    float rn = rd * WC / RC;
    float gn = grn * WC / GC;
    float bn = blu * WC / BC;
    //Find maximum white value that won’t desaturate
    float wn = min( rn, gn );
    wn = min( wn, bn );
    wn = min( wn, 255 ); //maximum white contribution reached.
    //Remove red, green, and blue contributions supplied by the white LED
    rn = rd - wn * RC / WC;
    gn = grn - wn * GC / WC;
    bn = blu - wn * BC / WC;
    //Cap red, green, and blue values at 255.
    r = min( rn, 255 );
    g = min( gn, 255 );
    b = min( bn, 255 );
    w = wn;
  }
  
	CRGBW(uint8_t rd, uint8_t grn, uint8_t blu, uint8_t wht) {
		r = rd;
		g = grn;
		b = blu;
		w = wht;
	}
  
  CRGBW(uint32_t colorcode) {
    r = ((colorcode >> 24) & 0xFF);
    g = ((colorcode >> 16) & 0xFF);
    b = ((colorcode >> 8) & 0xFF);
    w = ((colorcode >> 0) & 0xFF);
  }

	inline void operator = (const CRGB c) __attribute__((always_inline)) {
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;
		this->white = 0;
	}
};

inline uint16_t getRGBWsize(uint16_t nleds) {
	uint16_t nbytes = nleds * 4;
	if (nbytes % 3 > 0) return nbytes / 3 + 1;
	else return nbytes / 3;
}

#endif
