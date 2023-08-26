import telebot

# Replace 'YOUR_API_TOKEN' with your actual Telegram Bot API token
API_TOKEN = ''
bot = telebot.TeleBot(API_TOKEN)

@bot.message_handler(func=lambda message: True)
def print_chat_id(message):
    print("Chat ID:", message.chat.id)

bot.polling()
