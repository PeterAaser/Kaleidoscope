#include "hid.h"

#ifdef ARDUINO_VIRTUAL
#include "VirtualHID/VirtualHID.h"
#else
#include "KeyboardioHID.h"
#endif

#include "EventDispatcher.h"
#include "Kaleidoscope.h"

namespace kaleidoscope {
namespace hid {
void initializeKeyboard() {
  Keyboard.begin();
}

void pressRawKey(Key mappedKey) {
  EventDispatcher::eventDispatchers()
      .apply([&mappedKey](EventDispatcher *disp) {
               disp->keyPress(Kaleidoscope.connectionMask, mappedKey.keyCode);
             });
}

void _pressModifierKey(Key mappedKey) {
  pressRawKey(mappedKey);

  /* On at least ChromeOS 51-60, first sending a "press shift" key event in the same report
   * as the key report for some shifted keys causes the shift event to be missed.
   *
   * Specifically, Shift + [ in the same key report do not genereate a {
   *
   * This workaround causes an extra key report to be sent to toggle on the modifier key
   * before sending the key event we're trying to modify as part of a key report
   *
   * I (Jesse) don't believe we need a similar workaround for toggling the modifer _off_
   */

  if (!wasModifierKeyActive(mappedKey)) {
    sendKeyboardReport();
  }
}

void pressKey(Key mappedKey) {
  if (mappedKey.flags & SHIFT_HELD) {
    _pressModifierKey(Key_LeftShift);
  }
  if (mappedKey.flags & CTRL_HELD) {
    _pressModifierKey(Key_LeftControl);
  }
  if (mappedKey.flags & LALT_HELD) {
    _pressModifierKey(Key_LeftAlt);
  }
  if (mappedKey.flags & RALT_HELD) {
    _pressModifierKey(Key_RightAlt);
  }
  if (mappedKey.flags & GUI_HELD) {
    _pressModifierKey(Key_LeftGui);
  }

  pressRawKey(mappedKey);
}

void releaseRawKey(Key mappedKey) {
  EventDispatcher::eventDispatchers()
      .apply([&mappedKey](EventDispatcher *disp) {
               disp->keyRelease(Kaleidoscope.connectionMask, mappedKey.keyCode);
             });
}

void releaseAllKeys() {
  EventDispatcher::eventDispatchers()
      .apply([=](EventDispatcher *disp) {
               disp->keyReleaseAll(Kaleidoscope.connectionMask);
             });

  ConsumerControl.releaseAll();
}

void releaseKey(Key mappedKey) {
  if (mappedKey.flags & SHIFT_HELD) {
    releaseRawKey(Key_LeftShift);
  }
  if (mappedKey.flags & CTRL_HELD) {
    releaseRawKey(Key_LeftControl);
  }
  if (mappedKey.flags & LALT_HELD) {
    releaseRawKey(Key_LeftAlt);
  }
  if (mappedKey.flags & RALT_HELD) {
    releaseRawKey(Key_RightAlt);
  }
  if (mappedKey.flags & GUI_HELD) {
    releaseRawKey(Key_LeftGui);
  }
  releaseRawKey(mappedKey);
}

boolean isModifierKeyActive(Key mappedKey) {
  return Keyboard.isModifierActive(mappedKey.keyCode);
}

boolean wasModifierKeyActive(Key mappedKey) {
  return Keyboard.wasModifierActive(mappedKey.keyCode);
}

uint8_t getKeyboardLEDs() {
  return Keyboard.getLEDs();
}


void sendKeyboardReport() {
  EventDispatcher::eventDispatchers()
      .apply([](EventDispatcher *disp) {
               disp->keySendReport(Kaleidoscope.connectionMask);
             });

  //Keyboard.sendReport();
  ConsumerControl.sendReport();
}

void initializeConsumerControl() {
  ConsumerControl.begin();
}

void pressConsumerControl(Key mappedKey) {
  EventDispatcher::eventDispatchers()
      .apply([&mappedKey](EventDispatcher *disp) {
               disp->consumerPress(Kaleidoscope.connectionMask, CONSUMER(mappedKey));
             });
}

void releaseConsumerControl(Key mappedKey) {
  EventDispatcher::eventDispatchers()
      .apply([&mappedKey](EventDispatcher *disp) {
               disp->consumerRelease(Kaleidoscope.connectionMask, CONSUMER(mappedKey));
             });
}


void initializeSystemControl() {
  SystemControl.begin();
}

void pressSystemControl(Key mappedKey) {
  EventDispatcher::eventDispatchers()
      .apply([&mappedKey](EventDispatcher *disp) {
               disp->systemPress(Kaleidoscope.connectionMask, mappedKey.keyCode);
             });
}

void releaseSystemControl(Key mappedKey) {
  EventDispatcher::eventDispatchers()
      .apply([&mappedKey](EventDispatcher *disp) {
               disp->systemRelease(Kaleidoscope.connectionMask, mappedKey.keyCode);
             });
}


// Mouse events

void initializeMouse() {
  Mouse.begin();
}

void moveMouse(signed char x, signed char y, signed char vWheel, signed char hWheel) {
  Mouse.move(x, y, vWheel, hWheel);
}

void clickMouseButtons(uint8_t buttons) {
  Mouse.click(buttons);
}

void pressMouseButtons(uint8_t buttons) {
  Mouse.press(buttons);
}

void releaseMouseButtons(uint8_t buttons) {
  Mouse.release(buttons);
}

void releaseAllMouseButtons(void) {
  Mouse.releaseAll();
}

void sendMouseReport(void) {
  Mouse.sendReport();
}

/** SingleAbsolute mouse (grapahics tablet) events */

void initializeAbsoluteMouse() {
  SingleAbsoluteMouse.begin();
}

void moveAbsoluteMouse(signed char x, signed char y, signed char wheel) {
  SingleAbsoluteMouse.move(x, y, wheel);
}
void moveAbsoluteMouseTo(uint16_t x, uint16_t y, signed char wheel) {
  SingleAbsoluteMouse.moveTo(x, y, wheel);
}

void clickAbsoluteMouseButtons(uint8_t buttons) {
  SingleAbsoluteMouse.click(buttons);
}

void pressAbsoluteMouseButtons(uint8_t buttons) {
  SingleAbsoluteMouse.press(buttons);
}

void releaseAbsoluteMouseButtons(uint8_t buttons) {
  SingleAbsoluteMouse.release(buttons);
}

}
};
