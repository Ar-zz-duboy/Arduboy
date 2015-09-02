const unsigned char PROGMEM blob00_bitmap[] =
{
  //
  //
  //
  //
  //
  //
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


const unsigned char PROGMEM blob01_bitmap[] =
{
  //   #
  //  ###
  // #   #
  // ## ##
  //  ###
  //
  0x0C, 0x1A, 0x13, 0x1A, 0x0C, 0x00,
};

const unsigned char PROGMEM blob02_bitmap[] =
{
  //  ###
  // # # #
  // #####
  //  # #
  //  ###
  //
  0x06, 0x1D, 0x17, 0x1D, 0x06, 0x00,
};

const unsigned char PROGMEM blob03_bitmap[] =
{
  //  # #
  //  ###
  // #####
  // # # #
  //  ###
  //
  0x0C, 0x17, 0x1E, 0x17, 0x0C, 0x00,
};

const unsigned char PROGMEM blob04_bitmap[] =
{
  // #####
  //  # #
  //
  // #   #
  // #####
  //
  0x19, 0x13, 0x11, 0x13, 0x19, 0x00,
};

const unsigned char PROGMEM blob05_bitmap[] =
{
  // #####
  // # # #
  // #####
  // #####
  // #####
  //
  0x1F, 0x1D, 0x1F, 0x1D, 0x1F, 0x00,
};

const unsigned char *blobs_bitmap[] =
{
  blob00_bitmap, blob01_bitmap, blob02_bitmap, blob03_bitmap, blob04_bitmap, blob05_bitmap
};


const unsigned char PROGMEM blobsbackground_bitmap[] =
{
  //       #
  //  ##### 
  //  ##### 
  //  ##### 
  //  ##### 
  //  ##### 
  //  ##### 
  //  ##### 
  //  ##### 
  //  ##### 
  //  ##### 
  //  ##### 
  //       #
  0x00, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x01, 
  0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x10, 
};


const unsigned char PROGMEM blob00inverted_bitmap[] =
{
  // ######
  // ######
  // ######
  // ######
  // ######
  // ######
  0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
};

const unsigned char PROGMEM blob01inverted_bitmap[] =
{
  // ## ###
  // #   ##
  //  ### #
  //   #  #
  // #   ##
  // ######
  0x33, 0x25, 0x2C, 0x25, 0x33, 0x3F,
};

const unsigned char PROGMEM blob02inverted_bitmap[] =
{
  // #   ##
  //  # # #
  //      #
  // # # ##
  // #   ##
  // ######
  0x39, 0x22, 0x28, 0x22, 0x39, 0x3F,
};

const unsigned char PROGMEM blob03inverted_bitmap[] =
{
  // # # ##
  // #   ##
  //      #
  //  # # #
  // #   ##
  // ######
  0x33, 0x28, 0x21, 0x28, 0x33, 0x3F,
};

const unsigned char PROGMEM blob04inverted_bitmap[] =
{
  //      #
  // # # ##
  // ######
  //  ### #
  //      #
  // ######
  0x26, 0x2C, 0x2E, 0x2C, 0x26, 0x3F,
};

const unsigned char PROGMEM blob05inverted_bitmap[] =
{
  //      #
  //  # # #
  //      #
  //      #
  //      #
  // ######
  0x20, 0x22, 0x20, 0x22, 0x20, 0x3F,
};

const unsigned char *blobsinverted_bitmap[] =
{
  blob00inverted_bitmap, blob01inverted_bitmap, blob02inverted_bitmap, blob03inverted_bitmap, blob04inverted_bitmap, blob05inverted_bitmap
};


const unsigned char PROGMEM bigblobsbackground_bitmap[] =
{
  // #          #
  //  ########## 
  //  ########## 
  //  ########## 
  //  ########## 
  //  ########## 
  //  ########## 
  //  ########## 
  //  ########## 
  //  ########## 
  //  ########## 
  //             
  //             
  //  ########## 
  //  ########## 
  //  ########## 
  //  ########## 
  //  ########## 
  //  ########## 
  //  ########## 
  //  ########## 
  //  ########## 
  //  ########## 
  //             
  0x01, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x01, 
  0x00, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0x00, 
  0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00, 
};


const unsigned char PROGMEM bigblob00_bitmap[] =
{
  // ##########
  // ##########
  // ##########
  // ##########
  // ##########
  // ##########
  // ##########
  // ##########
  // ##########
  // ##########
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};


const unsigned char PROGMEM bigblob01_bitmap[] =
{
  // ####  ####
  // ####  ####
  // ##      ##
  // ##      ##
  //   ######  
  //   ######  
  //     ##    
  //     ##    
  // ##      ##
  // ##      ##
  0x0F, 0x0F, 0x33, 0x33, 0xF0, 0xF0, 0x33, 0x33, 0x0F, 0x0F, 
  0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 
};

const unsigned char PROGMEM bigblob02_bitmap[] =
{
  // ##      ##
  // ##      ##
  //   ##  ##  
  //   ##  ##  
  //           
  //           
  // ##  ##  ##
  // ##  ##  ##
  // ##      ##
  // ##      ##
  0xC3, 0xC3, 0x0C, 0x0C, 0xC0, 0xC0, 0x0C, 0x0C, 0xC3, 0xC3, 
  0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 
};

const unsigned char PROGMEM bigblob03_bitmap[] =
{
  // ##  ##  ##
  // ##  ##  ##
  // ##      ##
  // ##      ##
  //           
  //           
  //   ##  ##  
  //   ##  ##  
  // ##      ##
  // ##      ##
  0x0F, 0x0F, 0xC0, 0xC0, 0x03, 0x03, 0xC0, 0xC0, 0x0F, 0x0F, 
  0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 
};

const unsigned char PROGMEM bigblob04_bitmap[] =
{
  //           
  //           
  // ##  ##  ##
  // ##  ##  ##
  // ##########
  // ##########
  //   ######  
  //   ######  
  //           
  //           
  0x3C, 0x3C, 0xF0, 0xF0, 0xFC, 0xFC, 0xF0, 0xF0, 0x3C, 0x3C, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const unsigned char PROGMEM bigblob05_bitmap[] =
{
  //           
  //           
  //   ##  ##  
  //   ##  ##  
  //           
  //           
  //           
  //           
  //           
  //           
  0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const unsigned char *bigblobs_bitmap[] =
{
  bigblob00_bitmap, bigblob01_bitmap, bigblob02_bitmap, bigblob03_bitmap, bigblob04_bitmap, bigblob05_bitmap
};
