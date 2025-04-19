import asyncio 
from aiogram import Bot, Dispatcher, types

TOKEN = "7619575722:AAEMFSwzg-YRAQZx1_6cWxF0YIVWGdxPab8"

bot = Bot(token=TOKEN)
dp = Dispatcher()

# Обработчик сообщений (эхо-бот)
@dp.message()
async def echo(message: types.Message):
    await message.answer(message.text)  # Бот отвечает тем же текстом

# Запуск бота
async def main():
    await dp.start_polling(bot)

if __name__ == "__main__":
    asyncio.run(main())  # Правильный запуск бота в aiogram 3.x