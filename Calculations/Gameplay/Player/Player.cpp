#include "pch.h"
#include "Player.h"
#include <Graphics/Texture.h>
#include <Gameplay/Cards/Deck.h>


Player::Player()
{
    m_IsAlive = true;
    m_MaxHealth = 0;
    m_CurrentHealth = 0;
    m_NumbersHandSize = 0;
    m_OperandHandSize = 0;
    m_GoldCount = 0;
    m_Deck.ResetDeck();

    Texture::LoadPNG("Content/cat.png", m_Texture);

    m_OperandHand = std::vector<OperandCard*>();
    m_NumbersHand = std::vector<NumberCard*>();
}

Player::~Player()
{
    EmptyHands();
}

std::vector<OperandCard*>& Player::GetOperandHand()
{
    return m_OperandHand;
}

std::vector<NumberCard*>& Player::GetNumbersHand()
{
    return m_NumbersHand;
}

void Player::HardReset()
{
    m_Deck.ResetDeck();
    m_IsAlive = true;
    m_NumbersHandSize = INITIAL_PLAYER_NUMBER_HAND_SIZE;
    m_OperandHandSize = INITIAL_PLAYER_OPERAND_HAND_SIZE;
    m_GoldCount = INITIAL_PLAYER_GOLD;
    m_MaxHealth = INITIAL_PLAYER_HEALTH;
    m_CurrentHealth = m_MaxHealth;
}

void Player::IncreaseNumbersHandSize(const int& count)
{
    m_NumbersHandSize += count;
    m_NumbersHandSize = std::max(m_NumbersHandSize, 0);
}

const int Player::GetOperandHandSize() const
{
    return m_OperandHandSize;
}

const int Player::GetNumbersHandSize() const
{
    return m_NumbersHandSize;
}

void Player::IncreaseOperandsHandSize(const int& count)
{
    m_OperandHandSize += count;
    m_OperandHandSize = std::max(m_OperandHandSize, 0);
}

const int& Player::GetGoldCount() const
{
    return m_GoldCount;
}

const void Player::IncreaseGold(const int& count)
{
    m_GoldCount += count;
    m_GoldCount = std::max(m_GoldCount, 0);
}

const void Player::DecreaseGold(const int& count)
{
    m_GoldCount -= count;
    m_GoldCount = std::max(m_GoldCount, 0);
}

void Player::EmptyOperandHand()
{
    m_OperandHand.clear();
}

void Player::EmptyNumberHand()
{
    m_NumbersHand.clear();
}

void Player::EmptyHands()
{
    EmptyOperandHand();
    EmptyNumberHand();
}

void Player::DrawNumberCardsIntoHand(const int& count)
{
    m_Deck.DrawNumbersHand(count, m_NumbersHand);
}

void Player::DrawOperandCardsIntoHand(const int& count)
{
    m_Deck.DrawOperandCardsIntoHand(count, m_OperandHand);
}

void Player::RemoveCardFromHand(const Card* card)
{
    assert(card != nullptr);

    switch (card->GetCardType())
    {
    case CARD_TYPE::NUMBER_CARD:
    {
        RemoveNumbersCardFromHand((NumberCard*)(card));
    }
    break;

    case CARD_TYPE::OPERAND_CARD:
    {
        RemoveOperandCardFromHand((OperandCard*)(card));
    }
    break;

    default:
        assert(false);
        break;
    }
}

Deck& Player::GetDeck()
{
    return m_Deck;
}

void Player::RemoveOperandCardFromHand(const OperandCard* card)
{
    assert(card != nullptr);

    std::vector<OperandCard*>::iterator result = std::find(m_OperandHand.begin(), m_OperandHand.end(), card);

    //If triggered, trying to remove a card that doesnt exist in the hand.
    assert(result != m_OperandHand.end());

    if (result != m_OperandHand.end())
    {
        m_OperandHand.erase(result);
    }
}

void Player::RemoveNumbersCardFromHand(const NumberCard* card)
{
    assert(card != nullptr);

    std::vector<NumberCard*>::iterator result = std::find(m_NumbersHand.begin(), m_NumbersHand.end(), card);

    //If triggered, trying to remove a card that doesnt exist in the hand.
    assert(result != m_NumbersHand.end());

    if (result != m_NumbersHand.end())
    {
        m_NumbersHand.erase(result);
    }
}