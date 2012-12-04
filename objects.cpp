using namespace std;






struct Stat
{
	int current, max;
	int exp, exp_required;
	Stat(int c, int m) : current(c), max(m) {}
	void increase(int amount) {
		current += amount;
		if(current>max+5) current = max+5;
	}
	void update() {
		if(current>max) current--;
		if(current<max) current++;
	}
	int learn(int amount) {
		if(amount<1) return 0;
		exp += amount;
		int x = max;
		while(exp > exp_required) {
			max++;
			current++;
			exp -= exp_required;
			exp_required *= 1.1;
		}
		return max-x;
	} 
};

enum DamageType { physical = 0, fire = 1, poison = 2, ice = 3};

std::string DamageTypeName(DamageType type) {
	switch(type) {
		case physical: return "Physical";
		case fire: return "Fire";
		case poison: return "Poison";
		case ice: return "Ice";
		default: return "Unknown";
	}
}

class Action
{
public:
	// 0 = walk, 1 = sitdown, 2 = use ability, 3 = attack
	int id; 

	/* id = 0: 0 = left, 1 = top, 2 = right, 3 = bottom
		 id = 1: 0 = sitdown, 1 = situp
		 id = 2: charge (0-100)
		 id = 3: ...
	*/
	int info; 
	Ability* ability;
	Object* victim;
	ObjectPart* target; // e.g. arms or head
};

class Object
{
public:
	virtual ~Object() {}
	Stat hp;
	Action currentAction;
	map<DamageType,Stat> resistance;
	// damage = (base damage - trunc(resistance / 10)) * (resistance / (resistance + 100))
	
	virtual void damage(DamageType type, int amount) {
		int res = resistance[type].current;
		int true_damage = (amount - res/10) * res / (res + 100);
		if(true_damage>0) {
			hp.increase(-true_damage);
			onDamage(type, true_damage);
			if(hp.current <= 0) onDeath();
		}
	}

	virtual void onDamage(DamageType type, int amount) {}
	// what happens when hp drops below 0
	virtual void onDeath() {}

	void updateStats() {}
	void recalculateStats() {}
};

class LivingObject : public Object
{
public:
	virtual ~LivingObject() {}
	
	virtual void damage(DamageType type, int amount) {
		resistance[type].learn(amount);
		Object::damage(type,amount);
	}

	virtual void recalculateStats() {
		
	}
};


/* LUA */

class 'container' (Object)

function container:__init(item)
	Object.__init()
	self.item = item
end

function container:__onDeath()
	World.spawn(self.item, self.getLocation())
end

class 'living' (Object)

function living:__init()
	Object.__init()
end

function living:updateStats()
	
end
