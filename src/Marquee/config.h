// Main configuration
#define DELAY_INTERVAL 160  // the speed of marquee
#define DELAY_FINISH   2000
#define FLASH_INTERVAL 80   // the speed of animation when change string
#define FONT_SPACE 1        // how many blank lines between Chinese word
#define SS_SIZE 3           // how many LED matrix board

// Display characters
char *DISPLAY_WORDS[] = {"\033\036\037 \033\034\035 ",
                         "\031\032!! ",
                         "\026\027\030 ",
                         "I love Taiwan! ",
                         "Z>B ",
                         "We are Maker! ",
                         "Openlab.Taipei "};

#define LED_INDICATOR

#ifdef LED_INDICATOR
#  define LED_IDX_PIN 2
#  define LED_LLONG_INT 520
#  define LED_LONG_INT 360
#  define LED_SHORT_INT 120
#  define LED_DLEAY_INT 1000
#endif // LED_INDICATOR
