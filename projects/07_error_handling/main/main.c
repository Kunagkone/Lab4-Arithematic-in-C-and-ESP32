#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// 🏷️ Tag สำหรับ Log
static const char *TAG = "ERROR_HANDLING";

// 🚨 enum สำหรับประเภทข้อผิดพลาด
typedef enum {
    ERROR_NONE = 0,           // ไม่มีข้อผิดพลาด
    ERROR_DIVISION_BY_ZERO,   // หารด้วยศูนย์
    ERROR_INVALID_INPUT,      // ข้อมูลผิดประเภท
    ERROR_OUT_OF_RANGE,       // ข้อมูลเกินขอบเขต
    ERROR_NEGATIVE_VALUE,     // ค่าติดลบไม่เหมาะสม
    ERROR_OVERFLOW,           // ข้อมูลล้น
    ERROR_UNDERFLOW           // ข้อมูลต่ำเกินไป
} error_code_t;

// 📊 โครงสร้างผลลัพธ์
typedef struct {
    double result;
    error_code_t error;
    char message[100];
} calculation_result_t;

// 🎨 ฟังก์ชันแสดง ASCII Art ตามสถานการณ์
void show_ascii_art(error_code_t error) {
    switch(error) {
        case ERROR_NONE:
            ESP_LOGI(TAG, "   ✅ SUCCESS ✅");
            ESP_LOGI(TAG, "      🎉🎉🎉");
            ESP_LOGI(TAG, "    สำเร็จแล้ว!");
            break;
        case ERROR_DIVISION_BY_ZERO:
            ESP_LOGI(TAG, "   🍕 ÷ 0 = ❌");
            ESP_LOGI(TAG, "   😱 โอ้ะโอ!");
            ESP_LOGI(TAG, "  ไม่มีลูกค้า!");
            break;
        case ERROR_INVALID_INPUT:
            ESP_LOGI(TAG, "   📝 ABC บาท?");
            ESP_LOGI(TAG, "   🤔 งง...");
            ESP_LOGI(TAG, "  ตัวเลขหายไป");
            break;
        case ERROR_OUT_OF_RANGE:
            ESP_LOGI(TAG, "   📈 ∞∞∞∞∞");
            ESP_LOGI(TAG, "   😵 เกินขีด!");
            ESP_LOGI(TAG, "  ใหญ่เกินไป");
            break;
        default:
            ESP_LOGI(TAG, "   ❓ ERROR ❓");
            ESP_LOGI(TAG, "   🔧 แก้ไข");
            ESP_LOGI(TAG, "  ต้องตรวจสอบ");
    }
}

// 🛡️ ฟังก์ชันตรวจสอบการหารด้วยศูนย์
calculation_result_t safe_divide(double dividend, double divisor, const char* context) {
    calculation_result_t result = {0};
    
    ESP_LOGI(TAG, "\n🔍 ตรวจสอบการหาร: %s", context);
    ESP_LOGI(TAG, "📊 %g ÷ %g = ?", dividend, divisor);
    
    // ตรวจสอบหารด้วยศูนย์
    if (divisor == 0.0) {
        result.error = ERROR_DIVISION_BY_ZERO;
        strcpy(result.message, "❌ ข้อผิดพลาด: ไม่สามารถหารด้วยศูนย์ได้!");
        ESP_LOGE(TAG, "%s", result.message);
        show_ascii_art(ERROR_DIVISION_BY_ZERO);
        ESP_LOGI(TAG, "💡 แนะนำ: ตรวจสอบจำนวนลูกค้าก่อนแบ่งพิซซ่า");
        return result;
    }
    
    // ตรวจสอบผลลัพธ์ล้น
    result.result = dividend / divisor;
    if (isinf(result.result)) {
        result.error = ERROR_OVERFLOW;
        strcpy(result.message, "⚠️ เตือน: ผลลัพธ์เป็น infinity!");
        ESP_LOGW(TAG, "%s", result.message);
        return result;
    }
    
    // สำเร็จ
    result.error = ERROR_NONE;
    sprintf(result.message, "✅ สำเร็จ: %.2f ÷ %.2f = %.2f", dividend, divisor, result.result);
    ESP_LOGI(TAG, "%s", result.message);
    show_ascii_art(ERROR_NONE);
    
    return result;
}

// 💰 ฟังก์ชันตรวจสอบค่าเงิน
calculation_result_t validate_money(double amount, const char* description) {
    calculation_result_t result = {0};
    
    ESP_LOGI(TAG, "\n💰 ตรวจสอบเงิน: %s", description);
    ESP_LOGI(TAG, "💵 จำนวน: %.2f บาท", amount);
    
    // ตรวจสอบค่าติดลบ
    if (amount < 0) {
        result.error = ERROR_NEGATIVE_VALUE;
        strcpy(result.message, "❌ ข้อผิดพลาด: จำนวนเงินไม่สามารถติดลบได้!");
        ESP_LOGE(TAG, "%s", result.message);
        ESP_LOGI(TAG, "💡 แนะนำ: ตรวจสอบการคิดเงินใหม่");
        return result;
    }
    
    // ตรวจสอบเกินขีดจำกัด (1 ล้านล้าน)
    if (amount > 1000000000000.0) {
        result.error = ERROR_OUT_OF_RANGE;
        strcpy(result.message, "⚠️ เตือน: จำนวนเงินเกินขีดจำกัดระบบ!");
        ESP_LOGW(TAG, "%s", result.message);
        show_ascii_art(ERROR_OUT_OF_RANGE);
        ESP_LOGI(TAG, "💡 แนะนำ: ใช้ระบบธนาคารกลาง");
        return result;
    }
    
    // ตรวจสอบทศนิยมมากเกินไป
    double rounded = round(amount * 100) / 100;  // ปัดเศษสตางค์
    if (fabs(amount - rounded) > 0.001) {
        ESP_LOGW(TAG, "⚠️ เตือน: ปัดเศษจาก %.4f เป็น %.2f บาท", amount, rounded);
        amount = rounded;
    }
    
    result.result = amount;
    result.error = ERROR_NONE;
    sprintf(result.message, "✅ จำนวนเงินถูกต้อง: %.2f บาท", amount);
    ESP_LOGI(TAG, "%s", result.message);
    
    return result;
}

// 🔢 ฟังก์ชันตรวจสอบข้อมูลตัวเลข
calculation_result_t validate_number(const char* input, const char* field_name) {
    calculation_result_t result = {0};
    
    ESP_LOGI(TAG, "\n🔢 ตรวจสอบตัวเลข: %s", field_name);
    ESP_LOGI(TAG, "📝 ข้อมูลที่ป้อน: '%s'", input);
    
    // ตรวจสอบ NULL หรือ empty
    if (input == NULL || strlen(input) == 0) {
        result.error = ERROR_INVALID_INPUT;
        strcpy(result.message, "❌ ข้อผิดพลาด: ไม่มีข้อมูล!");
        ESP_LOGE(TAG, "%s", result.message);
        return result;
    }
    
    // ลองแปลงเป็นตัวเลข
    char* endptr;
    double value = strtod(input, &endptr);
    
    // ตรวจสอบว่าแปลงได้ทั้งหมดหรือไม่
    if (*endptr != '\0') {
        result.error = ERROR_INVALID_INPUT;
        sprintf(result.message, "❌ ข้อผิดพลาด: '%s' ไม่ใช่ตัวเลข!", input);
        ESP_LOGE(TAG, "%s", result.message);
        show_ascii_art(ERROR_INVALID_INPUT);
        ESP_LOGI(TAG, "💡 แนะนำ: ใช้เฉพาะตัวเลข 0-9 และจุดทศนิยม");
        return result;
    }
    
    // ตรวจสอบ NaN หรือ infinite
    if (isnan(value) || isinf(value)) {
        result.error = ERROR_INVALID_INPUT;
        strcpy(result.message, "❌ ข้อผิดพลาด: ตัวเลขไม่ถูกต้อง!");
        ESP_LOGE(TAG, "%s", result.message);
        return result;
    }
    
    result.result = value;
    result.error = ERROR_NONE;
    sprintf(result.message, "✅ ตัวเลขถูกต้อง: %.2f", value);
    ESP_LOGI(TAG, "%s", result.message);
    
    return result;
}

// 📊 ฟังก์ชันคำนวณดอกเบี้ยอย่างปลอดภัย
calculation_result_t calculate_interest(double principal, double rate, int years) {
    calculation_result_t result = {0};
    
    ESP_LOGI(TAG, "\n🏦 คำนวณดอกเบี้ย");
    ESP_LOGI(TAG, "💰 เงินต้น: %.2f บาท", principal);
    ESP_LOGI(TAG, "📈 อัตราดอกเบี้ย: %.2f%% ต่อปี", rate);
    ESP_LOGI(TAG, "⏰ ระยะเวลา: %d ปี", years);
    
    // ตรวจสอบเงินต้น
    if (principal <= 0) {
        result.error = ERROR_NEGATIVE_VALUE;
        strcpy(result.message, "❌ เงินต้นต้องมากกว่าศูนย์!");
        ESP_LOGE(TAG, "%s", result.message);
        return result;
    }
    
    // ตรวจสอบอัตราดอกเบี้ย
    if (rate < -100 || rate > 100) {
        result.error = ERROR_OUT_OF_RANGE;
        strcpy(result.message, "❌ อัตราดอกเบี้ยไม่สมเหตุสมผล!");
        ESP_LOGE(TAG, "%s", result.message);
        ESP_LOGI(TAG, "💡 แนะนำ: ใช้อัตรา -100% ถึง 100%");
        return result;
    }
    
    // ตรวจสอบระยะเวลา
    if (years < 0 || years > 100) {
        result.error = ERROR_OUT_OF_RANGE;
        strcpy(result.message, "❌ ระยะเวลาไม่สมเหตุสมผล!");
        ESP_LOGE(TAG, "%s", result.message);
        return result;
    }
    
    // คำนวณดอกเบี้ยแบบง่าย
    double interest = principal * (rate / 100.0) * years;
    double total = principal + interest;
    
    // ตรวจสอบ overflow
    if (total > DBL_MAX / 2) {
        result.error = ERROR_OVERFLOW;
        strcpy(result.message, "⚠️ เตือน: ผลลัพธ์ใหญ่เกินไป!");
        ESP_LOGW(TAG, "%s", result.message);
        return result;
    }
    
    result.result = total;
    result.error = ERROR_NONE;
    sprintf(result.message, "✅ ดอกเบี้ย: %.2f บาท, รวม: %.2f บาท", interest, total);
    ESP_LOGI(TAG, "%s", result.message);
    
    return result;
}

// 🍕 ฟังก์ชันจำลองสถานการณ์ร้านพิซซ่า
void pizza_shop_scenario(void) {
    ESP_LOGI(TAG, "\n🍕 === สถานการณ์ร้านพิซซ่า ===");
    ESP_LOGI(TAG, "📖 วันนี้ฝนตก ไม่มีลูกค้ามากิน");
    
    calculation_result_t result;
    
    // กรณีปกติ
    result = safe_divide(12, 4, "แบ่งพิซซ่า 12 ชิ้นให้ลูกค้า 4 คน");
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // กรณีมีปัญหา
    result = safe_divide(12, 0, "แบ่งพิซซ่า 12 ชิ้นให้ลูกค้า 0 คน");
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // กรณีฟื้นตัว
    ESP_LOGI(TAG, "\n🌞 ฝนหยุดแล้ว! มีลูกค้ามา 3 คน");
    result = safe_divide(12, 3, "แบ่งพิซซ่า 12 ชิ้นให้ลูกค้า 3 คน");
}

// 💰 ฟังก์ชันจำลองสถานการณ์ร้านขายของ
void shop_scenario(void) {
    ESP_LOGI(TAG, "\n🛒 === สถานการณ์ร้านขายของ ===");
    ESP_LOGI(TAG, "📖 เจ้าของร้านป้อนข้อมูลผิด");
    
    calculation_result_t result;
    
    // ตรวจสอบข้อมูลผิดประเภท
    result = validate_number("ABC", "ราคาสินค้า");
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    result = validate_number("12.50", "ราคาสินค้า");
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // ตรวจสอบเงินทอน
    result = validate_money(-50.0, "เงินทอน");
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    result = validate_money(25.75, "เงินทอน");
}

// 🏦 ฟังก์ชันจำลองสถานการณ์ธนาคาร
void bank_scenario(void) {
    ESP_LOGI(TAG, "\n🏦 === สถานการณ์ธนาคาร ===");
    ESP_LOGI(TAG, "📖 ลูกค้าฝากเงินและคำนวณดอกเบี้ย");
    
    calculation_result_t result;
    
    // กรณีปกติ
    result = calculate_interest(100000, 2.5, 5);
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // กรณีอัตราดอกเบี้ยติดลบ
    result = calculate_interest(100000, -5.0, 5);
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // กรณีเงินเกินขีดจำกัด
    result = validate_money(999999999999.0, "เงินฝาก");
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // กรณีแก้ไขแล้ว
    result = calculate_interest(100000, 3.0, 10);
}

// 📊 ฟังก์ชันสรุปความรู้
void show_error_handling_summary(void) {
    ESP_LOGI(TAG, "\n📚 === สรุปการจัดการข้อผิดพลาด ===");
    ESP_LOGI(TAG, "╔════════════════════════════════════════════╗");
    ESP_LOGI(TAG, "║              ประเภทข้อผิดพลาด             ║");
    ESP_LOGI(TAG, "╠════════════════════════════════════════════╣");
    ESP_LOGI(TAG, "║ 🚫 Division by Zero - หารด้วยศูนย์        ║");
    ESP_LOGI(TAG, "║ 📝 Invalid Input - ข้อมูลผิดประเภท       ║");
    ESP_LOGI(TAG, "║ 📊 Out of Range - เกินขอบเขต             ║");
    ESP_LOGI(TAG, "║ ➖ Negative Value - ค่าติดลบไม่เหมาะสม   ║");
    ESP_LOGI(TAG, "║ ⬆️ Overflow - ข้อมูลล้น                  ║");
    ESP_LOGI(TAG, "╚════════════════════════════════════════════╝");
    
    ESP_LOGI(TAG, "\n🛡️ === หลักการจัดการข้อผิดพลาด ===");
    ESP_LOGI(TAG, "✅ 1. ตรวจสอบข้อมูลก่อนคำนวณ");
    ESP_LOGI(TAG, "✅ 2. แสดงข้อความที่เข้าใจง่าย");
    ESP_LOGI(TAG, "✅ 3. ให้คำแนะนำในการแก้ไข");
    ESP_LOGI(TAG, "✅ 4. ป้องกันโปรแกรมค้างหรือ crash");
    ESP_LOGI(TAG, "✅ 5. ใช้ enum และ struct จัดการสถานะ");
}

void app_main(void) {
    ESP_LOGI(TAG, "🚀 เริ่มต้นโปรแกรมจัดการข้อผิดพลาด!");
    ESP_LOGI(TAG, "🛡️ การตรวจสอบและป้องกันข้อผิดพลาด\n");
    
    // รอสักครู่เพื่อให้ระบบเริ่มต้นเสร็จสิ้น
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // จำลองสถานการณ์ต่างๆ
    pizza_shop_scenario();
    vTaskDelay(pdMS_TO_TICKS(3000));
    
    shop_scenario();
    vTaskDelay(pdMS_TO_TICKS(3000));
    
    bank_scenario();
    vTaskDelay(pdMS_TO_TICKS(3000));
    
    // สรุปความรู้
    show_error_handling_summary();
    
    ESP_LOGI(TAG, "\n✅ เสร็จสิ้นการเรียนรู้การจัดการข้อผิดพลาด!");
    ESP_LOGI(TAG, "🎓 ได้เรียนรู้: enum, struct, error codes, และการตรวจสอบข้อมูล");
    ESP_LOGI(TAG, "🏆 ตอนนี้คุณสามารถเขียนโค้ดที่ปลอดภัยและน่าเชื่อถือแล้ว!");
}
   

// กำหนดรหัสข้อผิดพลาด
typedef enum {
    NO_ERROR,
    ERR_DIVIDE_BY_ZERO,
    ERR_NEGATIVE_BALANCE,
    ERR_INVALID_PRICE,
    ERR_OVERFLOW_AMOUNT,
    ERR_INVALID_INTEREST
} ErrorCode;

// ฟังก์ชันแสดงข้อความเตือน
void showErrorMessage(ErrorCode code) {
    switch(code) {
        case ERR_DIVIDE_BY_ZERO:
            printf("⚠️ ไม่สามารถหารด้วยศูนย์ได้ กรุณาระบุจำนวนลูกค้าใหม่\n");
            break;
        case ERR_NEGATIVE_BALANCE:
            printf("⚠️ จำนวนเงินติดลบ! โปรดตรวจสอบยอดเงินก่อน\n");
            break;
        case ERR_INVALID_PRICE:
            printf("⚠️ ราคาสินค้าไม่ถูกต้อง! กรุณาใส่ราคาเป็นตัวเลข\n");
            break;
        case ERR_OVERFLOW_AMOUNT:
            printf("⚠️ ยอดเงินฝากสูงเกินไป! เกินขีดจำกัดระบบ\n");
            break;
        case ERR_INVALID_INTEREST:
            printf("⚠️ อัตราดอกเบี้ยไม่ควรติดลบ! ตรวจสอบเงื่อนไขการคำนวณ\n");
            break;
        default:
            break;
    }
}

// ร้านพิซซ่า
ErrorCode checkPizzaShop(int pizza, int customers) {
    if (customers == 0)
        return ERR_DIVIDE_BY_ZERO;
    printf("🍕 พิซซ่าเฉลี่ยคนละ %d ชิ้น\n", pizza / customers);
    return NO_ERROR;
}

// ร้านขายของ
ErrorCode checkShop(float balance, char *priceStr) {
    if (balance < 0)
        return ERR_NEGATIVE_BALANCE;
    for (int i = 0; i < strlen(priceStr); i++) {
        if (priceStr[i] < '0' || priceStr[i] > '9')
            return ERR_INVALID_PRICE;
    }
    return NO_ERROR;
}

// ธนาคาร
ErrorCode checkBank(long long deposit, float interestRate) {
    if (deposit > 999999999)
        return ERR_OVERFLOW_AMOUNT;
    if (interestRate < 0)
        return ERR_INVALID_INTEREST;
    return NO_ERROR;
}

int main() {
    ErrorCode err;

    // ร้านพิซซ่า
    err = checkPizzaShop(12, 0);
    if (err != NO_ERROR) showErrorMessage(err);

    // ร้านขายของ
    err = checkShop(-50, "ABC");
    if (err != NO_ERROR) showErrorMessage(err);

    // ธนาคาร
    err = checkBank(999999999, -5.0);
    if (err != NO_ERROR) showErrorMessage(err);

    return 0;
}
