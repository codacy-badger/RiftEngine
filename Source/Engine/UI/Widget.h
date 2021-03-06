// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"

#include <vector>


#define W(Widget, ...) AddNew<Widget>(__VA_ARGS__)

class Widget : public Object {
	CLASS(Widget, Object)

	std::vector<GlobalPtr<Widget>> childs;

	bool bBuilding = true;
	bool bBuilt = true;

public:

	Widget() : Super(), bBuilding(false), bBuilt(false) {}

	/** Called after widget creation with parameters.
	 * For custom parameters create your own
	 * void Configure(...)
	 */
	virtual void Configure() {}

	/** called AFTER parent widget is built */
	void OnBuild() {
		if (!bBuilt && !bBuilding)
		{
			bBuilding = true;
			Build();
			bBuilding = false;
			bBuilt = true;

			for (auto& child : childs)
			{
				child->OnBuild();
			}
		}
	}

	void ForceRebuild() {
		if (!bBuilding)
		{
			bBuilt = false;
			UndoBuild();
			childs.clear();
			OnBuild();
		}
	}

	void OnTick() {
		if (bBuilt)
		{
			Tick();
		}
	}

	void TickChilds() {
		for (auto& child : childs)
		{
			child->OnTick();
		}
	}

protected:
	/** Create child widgets here with W() */
	virtual void Build() {}
	/** Called before rebuilding or when the widget gets destroyed */
	virtual void UndoBuild() {}

	virtual void Tick() { TickChilds(); }

	virtual void BeforeDestroy() override {
		UndoBuild();
		Super::BeforeDestroy();
	}


	/** Create widget from template type */
	template<typename WidgetType, typename... Args>
	static GlobalPtr<WidgetType> New(Args&&... args) {
		// Create the widget
		GlobalPtr<WidgetType> widget = Create<WidgetType>();

		// Configure it
		widget->Configure(std::forward<Args>(args)...);

		return widget;
	}

	/** Create widget from class */
	template<typename WidgetType, typename... Args>
	static GlobalPtr<WidgetType> New(Class* c, Args&&... args) {
		// Create the widget
		GlobalPtr<WidgetType> widget = Create<WidgetType>(c);

		widget->Configure(std::forward<Args>(args)...);

		return widget;
	}

	Ptr<Widget> Add(GlobalPtr<Widget>&& widget) {
		if (bBuilding && widget)
		{
			// Registry the widget
			Ptr<Widget> ptr = widget;
			childs.push_back(std::move(widget));

			return ptr;
		}
		return {};
	}

	template<typename WidgetType, typename... Args>
	Ptr<WidgetType> AddNew(Args&&... args) {
		return Add(New<Widget>(std::forward<Args>(args)...)).Cast<WidgetType>();
	}

public:

	template<typename WidgetType, typename... Args>
	static GlobalPtr<WidgetType> CreateStandalone(Args&&... args) {
		// Create the widget
		GlobalPtr<WidgetType> widget = Create<WidgetType>();

		// Configure it
		widget->Configure(std::forward<Args>(args)...);
		widget->OnBuild();
		return std::move(widget);
	}
};
