#include <algorithm>
#include <list>

using namespace std;

class Action
{
public:
	virtual void onPerform() {
	}
};

class ActionList
{
public:
	~ActionList() {
		for(Actions::iterator i = actions.begin(); i != actions.end(); ++i) {
			delete(i);
		}
	}
	void add(Action* action) {
		actions.push_back(action);
	}
	typedef list<Action*> Actions;
	Actions actions;
};

template <class Obj, class Type>
class Examine : public Action
{
public:
	Examine(Obj* obj_, Type* type_) :
		obj(obj_), type(type_) {}

	virtual void onPerform() {
		std::cout << type->onExamine(obj) << std::endl;
	}
private:
	Type* type;
	Obj* obj;
};

class Stack;

class Item
{
public:
	virtual bool isStackable() { return false; }
	virtual std::string onExamine(const Stack* s) = 0;
	virtual ActionList getActionList() {
		return ActionList();
	}
};

class StackableItem : public Item
{
public:
	virtual bool isStackable() { return true; }
};

class Stack
{
public:
	Stack(Item* item_) : item(item_), amount(1), quality(0) {}
	Stack(Item* item_, int amount_) : item(item_), amount(amount_), quality(0) {}
	Stack(Item* item_, int amount_, int quality_)
	: item(item_), amount(amount_), quality(quality_) {}

	Stack dropStack(Stack& s) {
		if(s.item == item && item->isStackable()) {
			s.amount += amount;
			s.quality = min(s.quality, quality);
		} else {
			swap(s);
		}
	}

	static Stack Empty() {
		Stack s;
		s.amount = 0;
		s.quality = 0;
		s.item = 0;
	}
	void swap(Stack& s) {
		swap(amount,s.amount);
		swap(quality,s.quality);
		swap(item,s.item);
	}
	int amount;
	int quality;
	Item* item;
	
	ActionList getActionList() {
		ActionList list = item->getActionList(this);
		list.add(new Examine<Stack,Item>(this,item));
	}
};

// Lua
// TODO: Does THIS work?
class 'EmptyBucket' (Action)
EmptyBucket:onPerform()
end
Items["Bucket"] = Item()
function Items["Bucket"]:onExamine(stack)
	return "An old wooden bucket"
end
function Items["WaterBucket"]:getActionList(stack_)
	return ActionList().add(
		CustomAction("Empty bucket",{stack = stack_}, function()
			print("You emptied the bucket")
			self.data.stack.changeItemType(Items["Bucket"])
		end)
end

