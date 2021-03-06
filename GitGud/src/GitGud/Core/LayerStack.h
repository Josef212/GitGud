#pragma once

#include "Core.h"
#include "Layer.h"

#include <vector>

namespace GitGud
{
	class GG_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return _layers.begin(); }
		std::vector<Layer*>::iterator end() { return _layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return _layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return _layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return _layers.begin(); }
		std::vector<Layer*>::const_iterator end() const { return _layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return _layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return _layers.rend(); }

	private:
		std::vector<Layer*> _layers;
		uint32_t _layerInsertIndex = 0;
	};
}
