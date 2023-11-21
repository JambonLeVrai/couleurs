#include "main.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_it.h"

#include <Adafruit_NeoPixel.h>
//#include <SPI.h>
#include "tools.h"
#include "wave_effect.h"
#include "cycle_effect.h"
#include "circular_wave_effect.h"
#include "compound_effect.h"
#include "SPI_processor.h"

#define PIN PB1
#define BUFFER_SIZE 64

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
SPIProcessor spi_processor;

uint8_t* buffer;
volatile bool started_receiving = false;
uint16_t buffer_size = 0;
uint16_t current_byte = 0;
volatile byte pos;
volatile bool process_it;

Vec2f key_coords[KEYS] = {
    Vec2f(0., 0.),
    Vec2f(1., 0.),
    Vec2f(2., 0.),
    Vec2f(3., 0.),
    Vec2f(0., 1.),
    Vec2f(1., 1.),
    Vec2f(2., 1.),
    Vec2f(3., 1.),
};

// Effect* effect = new WaveEffect(-45., 45., 0.25, 3., 5., Vec2f(3., 3.), color_from_rgb(185, 65, 0));
/* Effect* effect = new CycleEffect(std::vector<ColorF>(new std::vector<ColorF>({color_from_rgb(185, 65, 0),
                                                       color_from_rgb(122, 0, 0),
                                                       color_from_rgb(0, 122, 0),
                                                       color_from_rgb(0, 0, 122),
                                                       color_from_rgb(40, 40, 40)}), 4000.);*/
// Effect* effect = new CircularWaveEffect(1., 5., 3., Vec2f(1.5, 0.5), color_from_rgb(185, 65, 0), color_from_rgb(0, 0, 0));
/*Effect* effect = new CompoundEffect(std::vector<Effect*>({
  new WaveEffect(-45., 45., 0.25, 3., 5., Vec2f(3., 3.), color_from_rgb(140, 0, 0)),
  new CircularWaveEffect(1., 5., 3., Vec2f(1.5, 0.5), color_from_rgb(0, 140, 20), color_from_rgb(0, 0, 0))
}));*/
Effect *effect = new WaveEffect(-45., 45., 0.5, 3., 5., Vec2f(3., 3.), color_from_rgb(140, 0, 0), color_from_rgb(0, 140, 0));
// Effect* effect = new FixedEffect(150, 72, 12);

void colorSet(uint32_t c, uint8_t wait);

// SPI interrupt routine
/*ISR(SPI_STC_vect)
{
    if (buffer_length == 0) {
        buffer_length = SPDR;
        buffer = new uint8_t[buffer_length]();
        Serial.println(buffer_length);
        current_byte = 0;
    }
    else {
        buffer[current_byte] = SPDR;
        current_byte++;
        if (current_byte == buffer_length) {
            process_it = true;
        }
    }
}*/


uint8_t RX_Buffer[BUFFER_SIZE];
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_rx;


/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_SLAVE;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES_RXONLY;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_INPUT;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef * hspi) {
    HAL_SPI_Receive_DMA(&hspi1, RX_Buffer, BUFFER_SIZE);

    if(!started_receiving) {
        memcpy(&buffer_size, &RX_Buffer, 2);
        if(buffer_size == 0)
            return;
        else
            started_receiving = true;
        buffer = new uint8_t[buffer_size];
        current_byte = 0;
        if(buffer_size > 62) {
            memcpy(buffer, &RX_Buffer[2], BUFFER_SIZE - 2);
            current_byte += BUFFER_SIZE - 2;
        }
        else {
            memcpy(buffer, &RX_Buffer[2], buffer_size);
            started_receiving = false;
            process_it = true;
        }
    }
    else {
        if(current_byte + BUFFER_SIZE >= buffer_size) {
            memcpy(buffer + current_byte, &RX_Buffer[0], buffer_size - current_byte);
            process_it = true;
            started_receiving = false;
        }
        else {
            memcpy(buffer + current_byte, &RX_Buffer[0], BUFFER_SIZE);
            current_byte += BUFFER_SIZE;
        }
    }
}

int res;
/*
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
    if(HAL_SPI_GetError(hspi) == HAL_SPI_ERROR_OVR)
        digitalWrite(PC13, LOW);
}*/
int reset_pin_state = 0;

void setup()
{
    Serial.begin(115200);
    started_receiving = false;

    // have to send on master in, *slave out*
    /*pinMode(SS, INPUT_PULLUP);
    pinMode(MOSI, INPUT);
    pinMode(MISO, OUTPUT);
    pinMode(SCK, INPUT);*/
    //SPI.attachInterrupt(); // allows SPI interrupt

    strip.begin();

    strip.show(); // Initialize all pixels to 'off'

    HAL_Init();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_SPI1_Init();
    HAL_SPI_Receive_DMA(&hspi1, RX_Buffer, BUFFER_SIZE);
    //HAL_SPI_Receive_IT(&hspi1, RX_Buffer, BUFFER_SIZE);
    
    pinMode(PC13, OUTPUT);
    digitalWrite(PC13, HIGH);


    pos = 0;
    process_it = false;
}

void loop()
{
    if(digitalRead(PB4) == HIGH) {
        HAL_SPI_DMAStop(&hspi1);
        HAL_SPI_Receive_DMA(&hspi1, RX_Buffer, BUFFER_SIZE);
    }

    effect->refresh();
    for (int i = 0; i < 8; i++)
    {
        strip.setPixelColor(i, effect->get_color(key_coords[i], (uint8_t)i));
    }
    strip.show();

    if (process_it)
    {
        /*for(int i=0;i<buffer_size;i++) {
            switch(spi_processor.receiveData(buffer[i])) {
                case ProcessedType::NEW_EFFECT:
                    delete effect;
                    effect = spi_processor.getEffect();
                    break;
                case ProcessedType::KEY_PRESSED:
                    effect->key_pressed(spi_processor.keyPressed());
                    break;
                case ProcessedType::KEY_RELEASED:
                    effect->key_released(spi_processor.keyReleased());
                    break;
            }
        }*/
        switch(spi_processor.processData(buffer)) {
            case ProcessedType::NEW_EFFECT:
                delete effect;
                effect = spi_processor.getEffect();
                Serial.println("got new effect");
                Serial.println((int)effect);
                break;
            case ProcessedType::KEY_PRESSED:
                effect->key_pressed(spi_processor.keyPressed());
                break;
            case ProcessedType::KEY_RELEASED:
                effect->key_released(spi_processor.keyReleased());
                break;
        }
        process_it = false;
        buffer_size = 0;
        delete[] buffer;
    }

    delay(5);
}