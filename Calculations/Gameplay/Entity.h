#pragma once

class AnimationController;

class Entity
{
private:

protected:
	AnimationController* m_Animation;
	int m_CurrentHealth;
	int m_MaxHealth;
	bool m_IsAlive;

	Entity();
	bool LoadAnimation(const std::string& sheetPath, const unsigned int& numberOfAnimations, const unsigned int& frameCount, const float& duration, const bool& looping);
	void DestroyAnimation();

public:
	virtual	~Entity() = 0;

	AnimationController* GetAnimation() const;

	const void SetIsAlive(const bool& state);
	const bool GetIsAlive() const;
	const void Kill();

    const int& GetCurrentHealth() const;
    const int& GetMaxHealth() const;

    void TakeDamage(const int& damage);
    void Heal(const int& heal);
};

