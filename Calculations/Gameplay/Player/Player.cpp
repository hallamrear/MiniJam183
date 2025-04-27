#include "pch.h"
#include "Player.h"
#include <Graphics/Texture.h>
#include <Gameplay/Cards/Deck.h>

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

Player::Player()
{
    m_MaxHealth = INITIAL_PLAYER_HEALTH;
    m_CurrentHealth = m_MaxHealth;
    m_IsAlive = true;

    Texture::LoadPNG("Content/cat.png", m_Texture);

    m_Deck.ResetDeck();

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

void Player::FullReset()
{
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
