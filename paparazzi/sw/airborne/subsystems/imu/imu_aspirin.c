#include "subsystems/imu.h"

#include "mcu_periph/i2c.h"

struct ImuAspirin imu_aspirin;

/* initialize peripherals */
static void configure_gyro(void);
static void configure_mag(void);
static void configure_accel(void);


void imu_impl_init(void) {

  imu_aspirin.status = AspirinStatusUninit;
  imu_aspirin.gyro_available = FALSE;
  imu_aspirin.gyro_available_blaaa = FALSE;
  imu_aspirin.mag_ready_for_read = FALSE;
  imu_aspirin.mag_available = FALSE;
  imu_aspirin.accel_available = FALSE;

  imu_aspirin_arch_init();

}


void imu_periodic(void) {
  if (imu_aspirin.status == AspirinStatusUninit) {
    configure_gyro();
    configure_mag();
    configure_accel();
    imu_aspirin.status = AspirinStatusIdle;
  }
  else
    imu_aspirin.gyro_available_blaaa = TRUE;
}


/* sends a serie of I2C commands to configure the ITG3200 gyro */
static void configure_gyro(void) {

  struct i2c_transaction t;
  t.type = I2CTransTx;
  t.slave_addr = ITG3200_ADDR;
  /* set gyro range to 2000deg/s and low pass at 256Hz */
  t.buf[0] = ITG3200_REG_DLPF_FS;
  t.buf[1] = (0x03<<3);
  t.len_w = 2;
  i2c_submit(&i2c2,&t);
  while (t.status != I2CTransSuccess);
  /* set sample rate to 533Hz */
  t.buf[0] = ITG3200_REG_SMPLRT_DIV;
  t.buf[1] = 0x0E;
  i2c_submit(&i2c2,&t);
  while (t.status != I2CTransSuccess);
  /* switch to gyroX clock */
  t.buf[0] = ITG3200_REG_PWR_MGM;
  t.buf[1] = 0x01;
  i2c_submit(&i2c2,&t);
  while (t.status != I2CTransSuccess);
  /* enable interrupt on data ready, idle hight */
  t.buf[0] = ITG3200_REG_INT_CFG;
  t.buf[1] = (0x01 | 0x01<<7);
  i2c_submit(&i2c2,&t);
  while (t.status != I2CTransSuccess);

}

/* sends a serie of I2C commands to configure the ITG3200 gyro */
static void configure_mag(void) {

  struct i2c_transaction t;
  t.type = I2CTransTx;
  t.slave_addr = HMC5843_ADDR;
  /* set to rate to 50Hz */
  t.buf[0] = HMC5843_REG_CFGA;
  t.buf[1] = 0x00 | (0x06 << 2);
  i2c_submit(&i2c2,&t);
  while (t.status != I2CTransSuccess);
  /* set to gain to 1 Gauss */
  t.buf[0] = HMC5843_REG_CFGB;
  t.buf[1] = 0x01<<5;
  i2c_submit(&i2c2,&t);
  while (t.status != I2CTransSuccess);
  /* set to continuous mode */
  t.buf[0] = HMC5843_REG_MODE;
  t.buf[1] = 0x00;
  i2c_submit(&i2c2,&t);
  while (t.status != I2CTransSuccess);

}


static void send_i2c_msg_with_retry(struct i2c_transaction* t) {
  uint8_t max_retry = 8;
  uint8_t nb_retry = 0;
  do {
    i2c_submit(&i2c2,&t);
    while (t.status == I2CTransPending || t.status == I2CTransRunning);
    if (t.status == I2CTransFailed)
      nb_retry++;
  }
  while (t.status != I2CTransSuccess || nb_retry < max_retry);
}


static void configure_accel(void) {

  /* set data rate to 800Hz */
  adxl345_write_to_reg(ADXL345_REG_BW_RATE, 0x0D);
  /* switch to measurememnt mode */
  adxl345_write_to_reg(ADXL345_REG_POWER_CTL, 1<<3);
  /* enable data ready interrupt */
  adxl345_write_to_reg(ADXL345_REG_INT_ENABLE, 1<<7);
  /* Enable full res and interrupt active low */
  adxl345_write_to_reg(ADXL345_REG_DATA_FORMAT, 1<<3|1<<5);
  /* clear spi rx reg to make DMA happy */
  adxl345_clear_rx_buf();
  /* reads data once to bring interrupt line up */
  adxl345_start_reading_data();

}
