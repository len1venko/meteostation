import asyncio 
from aiogram import Bot, Dispatcher, types
from aiogram.types import ReplyKeyboardMarkup, KeyboardButton
from aiogram.filters import Command


TOKEN = "7639996461:AAE1Grm61BEjUb6uGqdIz1pvmTO5z4n6-Ak"


bot = Bot(token=TOKEN)
dp = Dispatcher()


# Главное меню
main_keyboard = ReplyKeyboardMarkup(
    keyboard=[
        [KeyboardButton(text="🖥️ LAN")],
        [KeyboardButton(text="🌍 WAN")],
        [KeyboardButton(text="🛜 AP")]
    ],
    resize_keyboard=True
)


# Подменю для LAN
lan_keyboard = ReplyKeyboardMarkup(
    keyboard=[
        [KeyboardButton(text="ℹ️ Переглянути історію показників мікроклімату (LAN)")],
        [KeyboardButton(text="🌤️ Переглянути дані про мікроклімат (LAN)")],
        [KeyboardButton(text="📊 Переглянути графік мікроклімату (LAN)")],
        [KeyboardButton(text="📅 Переглянути календар мікроклімату (LAN)")],
        [KeyboardButton(text="🔙 Назад")]
    ],
    resize_keyboard=True
)


# Подменю для WAN
wan_keyboard = ReplyKeyboardMarkup(
    keyboard=[
        [KeyboardButton(text="ℹ️ Переглянути історію показників мікроклімату (WAN)")],
        [KeyboardButton(text="🌤️ Переглянути дані про мікроклімат (WAN)")],
        [KeyboardButton(text="📊 Переглянути графік мікроклімату (WAN)")],
        [KeyboardButton(text="📅 Переглянути календар мікроклімату (WAN)")],
        [KeyboardButton(text="🔙 Назад")]
    ],
    resize_keyboard=True
)


# Подменю для AP
ap_keyboard = ReplyKeyboardMarkup(
    keyboard=[
        [KeyboardButton(text="ℹ️ Переглянути історію показників мікроклімату (AP)")],
        [KeyboardButton(text="🌤️ Переглянути дані про мікроклімат (AP)")],
        [KeyboardButton(text="📊 Переглянути графік мікроклімату (AP)")],
        [KeyboardButton(text="📅 Переглянути календар мікроклімату (AP)")],
        [KeyboardButton(text="🔙 Назад")]
    ],
    resize_keyboard=True
)


@dp.message(Command("start"))
async def start_handler(message: types.Message):
    await message.answer("Оберіть звідки ви бажаєте почати користування:", reply_markup=main_keyboard)


@dp.message()
async def menu_handler(message: types.Message):
    # Обрабатываем основное меню
    if message.text == "🖥️ LAN":
        await message.answer("Виберіть дію:", reply_markup=lan_keyboard)
    elif message.text == "🌍 WAN":
        await message.answer("Виберіть дію:", reply_markup=wan_keyboard)
    elif message.text == "🛜 AP":
        await message.answer("Виберіть дію:", reply_markup=ap_keyboard)
    

  # Кнопки LAN
    elif message.text == "ℹ️ Переглянути історію показників мікроклімату (LAN)":
        await message.answer("🔗 [Історія (LAN)](https://surl.li/harpcn)", parse_mode="Markdown", reply_markup=lan_keyboard)
    elif message.text == "🌤️ Переглянути дані про мікроклімат (LAN)":
        await message.answer("🔗 [Дані (LAN)](http://192.168.0.100)", parse_mode="Markdown", reply_markup=lan_keyboard)
    elif message.text == "📊 Переглянути графік мікроклімату (LAN)":
        await message.answer("🔗 [Графік (LAN)](http://192.168.0.100/index)", parse_mode="Markdown", reply_markup=lan_keyboard)
    elif message.text == "📅 Переглянути календар мікроклімату (LAN)":
        await message.answer("🔗 [Календар (LAN)](http://192.168.0.100/calendar)", parse_mode="Markdown", reply_markup=lan_keyboard)    


    # Кнопки WAN
    elif message.text == "ℹ️ Переглянути історію показників мікроклімату (WAN)":
        await message.answer("🔗 [Історія (WAN)](https://surl.li/harpcn)", parse_mode="Markdown", reply_markup=wan_keyboard)
    elif message.text == "🌤️ Переглянути дані про мікроклімат (WAN)":
        await message.answer("🔗 [Дані (WAN)](http://192.168.0.100:80)", parse_mode="Markdown", reply_markup=wan_keyboard)
    elif message.text == "📊 Переглянути графік мікроклімату (WAN)":
        await message.answer("🔗 [Графік (WAN)](http://192.168.0.100:80/index)", parse_mode="Markdown", reply_markup=wan_keyboard)
    elif message.text == "📅 Переглянути календар мікроклімату (LAN)":
        await message.answer("🔗 [Календар (LAN)](http://192.168.0.100:80/calendar)", parse_mode="Markdown", reply_markup=lan_keyboard)

   # Кнопки AP
    elif message.text == "ℹ️ Переглянути історію показників мікроклімату (AP)":
        await message.answer("🔗 [Історія (AP)](https://surl.li/harpcn)", parse_mode="Markdown", reply_markup=ap_keyboard)
    elif message.text == "🌤️ Переглянути дані про мікроклімат (AP)":
        await message.answer("🔗 [Дані (AP)](http://192.168.4.1)", parse_mode="Markdown", reply_markup=ap_keyboard)
    elif message.text == "📊 Переглянути графік мікроклімату (AP)":
        await message.answer("🔗 [Графік (AP)](http://192.168.4.1/index)", parse_mode="Markdown", reply_markup=ap_keyboard)
    elif message.text == "📅 Переглянути календар мікроклімату (AP)":
        await message.answer("🔗 [Календар (AP)](http://192.168.4.1/calendar)", parse_mode="Markdown", reply_markup=ap_keyboard)    

    # Назад в главное меню
    elif message.text == "🔙 Назад":
        await message.answer("Оберіть потрібну дію:", reply_markup=main_keyboard)
    else:
        await message.answer("Я не розумію цю команду. Будь ласка, оберіть опцію з меню.")


async def main():
    await dp.start_polling(bot)


if __name__ == "__main__":
    asyncio.run(main())
