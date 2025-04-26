#include "pch.h"
#include "Player.h"
#include <Graphics/Texture.h>
#include <Gameplay/Cards/Deck.h>

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

const std::vector<OperandCard*>& Player::GetOperandHand() const
{
    return m_OperandHand;
}

const std::vector<NumberCard*>& Player::GetNumbersHand() const
{
    return m_NumbersHand;
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

Deck& Player::GetDeck()
{
    return m_Deck;
}
