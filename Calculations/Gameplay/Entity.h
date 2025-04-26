#pragma once

class Entity
{
private:

protected:
	SDL_Texture* m_Texture;
	int m_CurrentHealth;
	int m_MaxHealth;
	bool m_IsAlive;

	Entity();

public:
	virtual	~Entity() = 0;

	SDL_Texture* GetTexture() const;

	const void SetIsAlive(const bool& state);
	const bool GetIsAlive() const;
	const void Kill();

    const int& GetCurrentHealth() const;
    const int& GetMaxHealth() const;

    void TakeDamage(const int& damage);
    void Heal(const int& heal);
};

