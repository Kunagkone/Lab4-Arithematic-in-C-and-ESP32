#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "TOYS_MATH";

void app_main(void)
{
    ESP_LOGI(TAG, "🧸 เริ่มต้นโปรแกรมนับของเล่นของน้อง 🧸");
    ESP_LOGI(TAG, "=========================================");
    
    // ประกาศตัวแปรเก็บจำนวนของเล่น
    int toys_have = 15;        // ของเล่นที่น้องมีอยู่
    int toys_give_away = 7;   // ของเล่นที่แจกให้เพื่อน
    int toys_remaining;       // ของเล่นที่เหลือ
    if (toys_at_home >= toys_give_away) {
            ESP_LOGI(TAG, "✅ ของเล่นพอแจก");
   } else {
        ESP_LOGI(TAG, "❌ ของเล่นไม่พอ! ขาดไป %d ชิ้น", 
            toys_give_away - toys_at_home);
}
    // แสดงข้อมูลเริ่มต้น
    ESP_LOGI(TAG, "📖 โจทย์:");
    ESP_LOGI(TAG, "   น้องมีของเล่น: %d ชิ้น", toys_have);
    ESP_LOGI(TAG, "   เอาไปแจกให้เพื่อน: %d ชิ้น", toys_give_away);
    ESP_LOGI(TAG, "   ❓ น้องเหลือของเล่นกี่ชิ้น?");
    ESP_LOGI(TAG, "");
    
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    
    // ตรวจสอบก่อนคำนวณ
    if (toys_give_away > toys_have) {
        ESP_LOGW(TAG, "⚠️  คำเตือน: ของเล่นที่จะแจก (%d) มากกว่าที่มีอยู่ (%d)!", 
                 toys_give_away, toys_have);
        ESP_LOGI(TAG, "   น้องไม่สามารถแจกของเล่นได้มากกว่าที่มี");
        ESP_LOGI(TAG, "   จะแจกได้เฉพาะ %d ชิ้น (ทั้งหมดที่มี)", toys_have);
        toys_give_away = toys_have;
    }
    
    // คำนวณผลลัพธ์ (การลบ)
    toys_remaining = toys_have - toys_give_away;
    
    // แสดงขั้นตอนการคิด
    ESP_LOGI(TAG, "🧮 ขั้นตอนการคิด:");
    ESP_LOGI(TAG, "   ของเล่นที่มี - ของเล่นที่แจก");
    ESP_LOGI(TAG, "   = %d - %d", toys_have, toys_give_away);
    ESP_LOGI(TAG, "   = %d ชิ้น", toys_remaining);
    ESP_LOGI(TAG, "");
    
    // แสดงคำตอบ
    ESP_LOGI(TAG, "✅ คำตอบ:");
    ESP_LOGI(TAG, "   น้องเหลือของเล่น %d ชิ้น", toys_remaining);
    ESP_LOGI(TAG, "");
    
    // แสดงภาพประกอบ
    ESP_LOGI(TAG, "🎨 ภาพประกอบ:");
    ESP_LOGI(TAG, "   ของเล่นเดิม: 🧸🚗🎲🧩🎮🧸🚁🎯 (%d ชิ้น)", toys_have);
    ESP_LOGI(TAG, "   แจกให้เพื่อน: 🧸🚗🎲 (%d ชิ้น)", toys_give_away);
    ESP_LOGI(TAG, "   เหลืออยู่:   🧩🎮🧸🚁🎯 (%d ชิ้น)", toys_remaining);
    ESP_LOGI(TAG, "");
    
    int dolls = 5;        // ตุ๊กตา
    int robots = 2;       // หุ่นยนต์
    int total_toys = toys_at_home + dolls + robots;

    ESP_LOGI(TAG, "🪆 ตุ๊กตา: %d ตัว", dolls);
    ESP_LOGI(TAG, "🤖 หุ่นยนต์: %d ตัว", robots);
    ESP_LOGI(TAG, "🎯 ของเล่นทั้งหมด: %d ชิ้น", total_toys);

    // ตัวอย่างเพิ่มเติม
    ESP_LOGI(TAG, "💡 ตัวอย่างเพิ่มเติม:");
    
    // ตัวอย่างที่ 1
    int ex1_have = 15;
    int ex1_give = 7;
    int ex1_remain = ex1_have - ex1_give;
    ESP_LOGI(TAG, "   ถ้าน้องมีของเล่น %d ชิ้น และแจกไป %d ชิ้น", ex1_have, ex1_give);
    ESP_LOGI(TAG, "   จะเหลือ %d - %d = %d ชิ้น", ex1_have, ex1_give, ex1_remain);
    ESP_LOGI(TAG, "");
    
    // ตัวอย่างที่ 2 (กรณีพิเศษ)
    int ex2_have = 5;
    int ex2_give = 5;
    int ex2_remain = ex2_have - ex2_give;
    ESP_LOGI(TAG, "   ถ้าน้องมีของเล่น %d ชิ้น และแจกไปหมด %d ชิ้น", ex2_have, ex2_give);
    ESP_LOGI(TAG, "   จะเหลือ %d - %d = %d ชิ้น (ไม่เหลือเลย)", ex2_have, ex2_give, ex2_remain);
    ESP_LOGI(TAG, "");
    
    // เปรียบเทียบกับการบวก
    ESP_LOGI(TAG, "🔄 เปรียบเทียบกับการบวก:");
    ESP_LOGI(TAG, "   การบวก: เพิ่มจำนวน (เช่น ไข่ 4 + 2 = 6)");
    ESP_LOGI(TAG, "   การลบ: ลดจำนวน (เช่น ของเล่น 8 - 3 = 5)");
    ESP_LOGI(TAG, "   ข้อแตกต่าง: การลบต้องระวังไม่ให้ติดลบ!");
    ESP_LOGI(TAG, "");
    
    // กรณีที่อาจเกิดปัญหา
    ESP_LOGI(TAG, "⚠️  กรณีที่ต้องระวัง:");
    int danger_have = 3;
    int danger_give = 5;
    ESP_LOGI(TAG, "   ถ้าน้องมีของเล่น %d ชิ้น แต่จะแจก %d ชิ้น", danger_have, danger_give);
    ESP_LOGI(TAG, "   จะได้ %d - %d = %d (ผลลัพธ์เป็นลบ!)", 
             danger_have, danger_give, danger_have - danger_give);
    ESP_LOGI(TAG, "   ในชีวิตจริง: น้องไม่สามารถแจกของเล่นมากกว่าที่มีได้");
    ESP_LOGI(TAG, "");
    
    // สรุปการเรียนรู้
    ESP_LOGI(TAG, "📚 สิ่งที่เรียนรู้:");
    ESP_LOGI(TAG, "   1. การลบเลข (Subtraction): a - b = c");
    ESP_LOGI(TAG, "   2. การตรวจสอบเงื่อนไข (if statement)");
    ESP_LOGI(TAG, "   3. การจัดการกรณีพิเศษ (edge cases)");
    ESP_LOGI(TAG, "   4. ความแตกต่างระหว่างการบวกและการลบ");
    ESP_LOGI(TAG, "   5. การป้องกันผลลัพธ์ที่ไม่สมเหตุสมผล");
    ESP_LOGI(TAG, "");
    
    ESP_LOGI(TAG, "🎉 จบโปรแกรมนับของเล่นของน้อง!");
    ESP_LOGI(TAG, "📖 อ่านต่อในโปรเจคถัดไป: 03_multiplication_candies");
    
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    int friends = 10;
    int toys_per_friend = 2;
    int total_needed = friends * toys_per_friend;

    int available = 15;  
    int missing = 0;

    printf("ต้องมีของเล่นทั้งหมด: %d ชิ้น\n", total_needed);

    if (available < total_needed) {
        missing = total_needed - available;
        printf("มีของเล่น %d ชิ้น ขาดอีก %d ชิ้น\n", available, missing);
    } else {
        printf("มีของเล่น %d ชิ้น เพียงพอสำหรับแจก\n", available);
    }
        return 0;
} 
