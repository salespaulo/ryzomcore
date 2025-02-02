// Ryzom - MMORPG Framework <http://dev.ryzom.com/projects/ryzom/>
// Copyright (C) 2010  Winch Gate Property Limited
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "stdpch.h"

#include "nel/gui/html_element.h"
#include "nel/gui/libwww.h"

using namespace std;
using namespace NLMISC;

#ifdef DEBUG_NEW
#define new DEBUG_NEW
#endif

namespace NLGUI
{
	// ***************************************************************************
	CHtmlElement::CHtmlElement(ENodeType type, std::string value)
		: ID(0), Type(type), Value(value), parent(NULL), previousSibling(NULL), nextSibling(NULL), childIndex(0)
	{}

	// ***************************************************************************
	bool CHtmlElement::hasAttribute(const std::string &key) const
	{
		return Attributes.find(key) != Attributes.end();
	}

	// ***************************************************************************
	bool CHtmlElement::hasNonEmptyAttribute(const std::string &key) const
	{
		return !getAttribute(key).empty();
	}

	// ***************************************************************************
	std::string CHtmlElement::getAttribute(const std::string &key) const
	{
		std::map<std::string, std::string>::const_iterator it = Attributes.find(key);
		return (it != Attributes.end() ? it->second : "");
	}

	// ***************************************************************************
	bool CHtmlElement::hasClass(const std::string &key) const
	{
		return ClassNames.find(key) != ClassNames.end();
	}

	// ***************************************************************************
	void CHtmlElement::reindexChilds()
	{
		uint index = 0;
		CHtmlElement *prev = NULL;
		std::list<CHtmlElement>::iterator it;
		for(it = Children.begin(); it != Children.end(); ++it)
		{
			if (it->Type == ELEMENT_NODE)
			{
				it->parent = this;
				it->previousSibling = prev;
				it->nextSibling = NULL;
				if (prev)
				{
					prev->nextSibling = &(*it);
				}
				it->childIndex = index;
				index++;
				prev = &(*it);
			}
		}
	}

	// ***************************************************************************
	void CHtmlElement::clearPseudo()
	{
		_Pseudo.clear();
	}

	// ***************************************************************************
	bool CHtmlElement::hasPseudo(const std::string &key) const
	{
		return _Pseudo.find(key) != _Pseudo.end();
	}

	// ***************************************************************************
	TStyle CHtmlElement::getPseudo(const std::string &key) const
	{
		std::map<std::string, TStyle>::const_iterator it = _Pseudo.find(key);
		if (it != _Pseudo.end())
			return it->second;

		return TStyle();
	}

	// ***************************************************************************
	void CHtmlElement::setPseudo(const std::string &key, const TStyle &style)
	{
		std::map<std::string, TStyle>::iterator it = _Pseudo.find(key);
		if (it != _Pseudo.end())
		{
			// insert into previous, override previous values if they exist
			for(TStyle::const_iterator itStyle = style.begin(); itStyle != style.end(); ++itStyle)
			{
				it->second[itStyle->first] = itStyle->second;
			}
		}
		else
		{
			_Pseudo[key] = style;
		}
	}

	// ***************************************************************************
	std::string CHtmlElement::getInheritedLanguage() const
	{
		const CHtmlElement *node = this;
		while(node)
		{
			if (node->hasAttribute("lang"))
				return node->getAttribute("lang");

			node = node->parent;
		}

		return "";
	}

	// ***************************************************************************
	std::string CHtmlElement::htmlEscape(std::string val, bool isAttribute) const
	{
		static const std::string searchReplace[] = {
			"&", "&amp;",
			"<", "&lt;",
			">", "&gt;",
			"\xA0", "&nbsp;",
		};
		
		for(uint i = 0; i < (sizeof(searchReplace) / sizeof(searchReplace[0])); i+=2)
			val = strFindReplaceAll(val, searchReplace[i], searchReplace[i+1]);

		if (isAttribute)
		{
			static const std::string q = "\"";
			static const std::string quot = "&quot;";
			val = strFindReplaceAll(val, q, quot);
		}

		return val;
	}

	// ***************************************************************************
	std::string CHtmlElement::serializeAttributes() const
	{
		std::string result;
		for(std::map<std::string, std::string>::const_iterator it = Attributes.begin(); it != Attributes.end(); ++it)
		{
			if (it->first == "class")
			{
				result += " class=\"";
				for(std::set<std::string>::const_iterator it2 = ClassNames.begin(); it2 != ClassNames.end(); ++it2)
				{
					if (it2 != ClassNames.begin())
					{
						result += " ";
					}
					result += htmlEscape(*it2, true);
				}
				result += "\"";
			}
			else
			{
				result += " " + it->first + "=\"" + htmlEscape(it->second, true) + "\"";
			}
		}
		return result;
	}

	// ***************************************************************************
	std::string CHtmlElement::serializeChilds() const
	{
		std::string result;
		for(std::list<CHtmlElement>::const_iterator it = Children.begin(); it != Children.end(); ++it)
			result += it->serialize();

		return result;
	}

	// ***************************************************************************
	std::string CHtmlElement::serialize() const
	{
		if (Type == TEXT_NODE)
		{
			if (parent && (parent->ID == HTML_SCRIPT || parent->ID == HTML_STYLE ||
				parent->ID == HTML_IFRAME || parent->ID == HTML_NOEMBED ||
				parent->ID == HTML_NOSCRIPT))
			{
				return Value;
			} else {
				return htmlEscape(Value);
			}
		}

		std::string result = "<" + Value + serializeAttributes() + ">";

		if (ID == HTML_AREA || ID == HTML_BASE || ID == HTML_BR ||
			ID == HTML_COL || ID == HTML_EMBED || ID == HTML_HR ||
			ID == HTML_IMG || ID == HTML_INPUT || ID == HTML_LINK ||
			ID == HTML_META || ID == HTML_PARAM || ID == HTML_WBR)
		{
			return result;
		}

		// first linebreak that will be ignored on parse time
		if (ID == HTML_PRE || ID == HTML_TEXTAREA)
			result += "\n";

		if (!Children.empty())
			result += serializeChilds();

		result += "</" + Value + ">";

		return result;
	}

	// ***************************************************************************
	std::string CHtmlElement::toString(bool tree, uint depth) const
	{
		std::string result;
		if (depth > 0)
		{
			result = NLMISC::toString("[%d]", depth);
			result.append(depth, '-');
		}
		if (Type == NONE || Type == ELEMENT_NODE)
		{
			result += "<" + Value;
			for(std::map<std::string, std::string>::const_iterator it = Attributes.begin(); it != Attributes.end(); ++it)
			{
				if (it->first == "class")
				{
					result += " class=\"";
					for(std::set<std::string>::const_iterator it2 = ClassNames.begin(); it2 != ClassNames.end(); ++it2)
					{
						if (it2 != ClassNames.begin())
						{
							result += " ";
						}
						result += *it2;
					}
					result += "\"";
				}
				else
				{
					result += " " + it->first + "=\"" + it->second + "\"";
				}
			}
			result += NLMISC::toString(" data-debug=\"childIndex: %d\"", childIndex);
			result += ">";

			if (tree)
			{
				result += "\n";
				for(std::list<CHtmlElement>::const_iterator it = Children.begin(); it != Children.end(); ++it)
				{
					result += it->toString(tree, depth+1);
				}
				if (depth > 0)
				{
					result += NLMISC::toString("[%d]", depth);
					result.append(depth, '-');
				}
				result += "</" + Value + ">\n";
			}
		}
		else
		{
			if (Value.find("\n") == std::string::npos)
			{
				result += "{" + Value + "}";
			}
			else
			{
				result += "{";
				std::string::size_type start = 0;
				std::string::size_type pos = Value.find_first_of("\n\r\t");
				while(pos != std::string::npos)
				{
					result += Value.substr(start, pos - start);
					if (Value[pos] == '\n')
					{
						result += "\xE2\x8F\x8E"; // \u23CE
					}
					else if (Value[pos] == '\t')
					{
						result += "\xE2\x87\xA5"; // \u21E5
					}

					start = pos+1;
					pos = Value.find_first_of("\n\r\t", start);
				}
				result += "}";
			}
			if (tree) result += "\n";
		}

		return result;
	};
} // namespace
