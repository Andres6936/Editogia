// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_KEY_HPP
#define EDITOGIA_KEY_HPP

#include "Editogia/IO/User/KeyCode.hpp"

namespace Editogia
{

	/**
	 * Key data : special code or character
	 */
	class Key
	{

	private:

		KeyCode keyCode = KeyCode::NONE;

		bool pressed = false;

		bool release = false;

		bool leftAltPressed = false;

		bool leftCtrlPressed = false;

		bool rightAltPressed = false;

		bool rightCtrlPressed = false;

		bool shift = false;

	public:

		// Methods

		void resetState() noexcept;

		// Getters

		KeyCode getKeyCode() const noexcept;

		bool isPressed() const noexcept;

		bool isRelease() const noexcept;

		bool isLeftAltPressed() const noexcept;

		bool isLeftCtrlPressed() const noexcept;

		bool isRightAltPressed() const noexcept;

		bool isRightCtrlPressed() const noexcept;

		bool isShift() const noexcept;

		// Setter

		void setKeyCode(KeyCode _keyCode) noexcept;

		void setPressed(bool _pressed) noexcept;

		void setRelease(bool _release) noexcept;

		void setLeftAltPressed(bool _leftAltPressed) noexcept;

		void setLeftCtrlPressed(bool _leftCtrlPressed) noexcept;

		void setRightAltPressed(bool _rightAltPressed) noexcept;

		void setRightCtrlPressed(bool _rightCtrlPressed) noexcept;

		void setShift(bool _shift) noexcept;

	};

}

#endif //EDITOGIA_KEY_HPP
