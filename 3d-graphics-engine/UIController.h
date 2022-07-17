#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include "Constants.h"
#include <vector>
#include <utility>
#include <map>

class GraphicsEngine;
class Light;
class Entity;
struct SystemData;

// This class accepts strings and stores them.
// If the same string is given, this will be identified by a (n)
// suffix when retrieved, where n is the count after the original one
class StringCounter
{
	std::vector<std::string> m_strings;
public:
	StringCounter() = default;

	void Add(const std::string& name)
	{
		m_strings.push_back(name);
	}

	std::vector<std::string> GetStrings() const
	{
		std::vector<std::string> stringsWithDupes;
		std::map<std::string, int> freqMap;

		for(const auto& str : m_strings)
		{
			// Check to see if we have seen it before
			// Search for it in the freq map
			auto it = freqMap.find(str);

			// Assign the default name
			std::string name = str;

			// Have we seen this name before?
			if(it != freqMap.end())
			{
				// If so append the appropriate count to the end
				// so we can uniquely identify it
				int count = it->second;
				if(count > 0)
				{
					name += "(" + std::to_string(count) + ")";
				}

				// Update the count
				it->second += 1;
			}
			else
			{
				// Never been seen before, add it to the map			
				freqMap.insert({ str, 1 });
			}

			// Add the modified string to the vector
			stringsWithDupes.push_back(name);
		}
		return stringsWithDupes;
	}

	int GetSize() const { return m_strings.size(); }
};

class UIController
{
	const std::vector<Entity*>* m_pEntities;
	const std::array<Light*, Constants::MAX_LIGHTS>* m_pLights;
	const SystemData& m_systemData;

	// Stores names of all entities and lights, allowing duplicates
	// but with a suffix at the end showing its 'count' after the original
	StringCounter m_entFreq;
	StringCounter m_lightFreq;

	std::vector<const char*> m_entNames;
	std::vector<const char*> m_lightNames;

public:
	UIController(GraphicsEngine* pEngine);
	~UIController();

	// Stores a name in a specific category (Entities/Lights)
	// so it can be used for the UI.
	void AddSceneObject(const std::string& name);

	// Fetch the strings and store them appropriately so ImGui is happy with them
	// once we know now more entities will be added, instead of doing it each frame.
	void BeforeFirstRender(int numEntites, int numLights);

	void DrawUI(GraphicsEngine* pEngine) const;
};

#endif
