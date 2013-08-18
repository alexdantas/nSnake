#include "Menu.hpp"
#include "Input.hpp"
#include "Ncurses.hpp"

Menu::Menu(unsigned long color, unsigned long hiliteColor, int x, int y):
    color(color),
    hiliteColor(hiliteColor),
    item(),
    items(0),
    currentItem(0),
    x(x),
    y(y),
    width(0),
    height(0),
    mouseSelectedItem(false)
{ }
Menu::~Menu()
{
    for (int i = 0; i < (this->items); i++)
        if (this->item[i])
            delete this->item[i];
}
void Menu::addItem(std::string text, int value)
{
    MenuItem* tmpItem = new MenuItem(text,
                                     value,
                                     this->x,
                                     (this->items + this->y));

    this->item.push_back(tmpItem);
    this->items++;

    if (text.length() > (this->width))
        this->width = text.length();

    this->height++;

    // if (this->items == 1)
    //     this->item[0]->text->setFont(this->hiliteColor);
}
int Menu::getCurrentItemValue()
{
    return (this->item[currentItem]->value);
}
int Menu::getCurrentItemIndex()
{
    return (this->currentItem);
}
int Menu::getAmmount()
{
    return (this->items);
}
void Menu::next()
{
    if (this->currentItem >= (this->items - 1))
    {
        this->first(); // I could simply block the item
        return;        // from going, but it's nice to
    }                  // wrap up the menu.

    //   this->item[currentItem]->text->setFont(this->font);
    this->currentItem++;
//    this->item[currentItem]->text->setFont(this->hiliteColor);
}
void Menu::previous()
{
    if (this->currentItem <= 0)
    {
        this->last();
        return;
    }
//    this->item[currentItem]->text->setFont(this->font);
    this->currentItem--;
//    this->item[currentItem]->text->setFont(this->hiliteColor);
}
void Menu::first()
{
//    this->item[currentItem]->text->setFont(this->font);
    this->currentItem = 0;
//    this->item[currentItem]->text->setFont(this->hiliteColor);
}
void Menu::last()
{
//    this->item[currentItem]->text->setFont(this->font);
    this->currentItem = (items - 1); // counts from zero
//    this->item[currentItem]->text->setFont(this->hiliteColor);
}
void Menu::render()
{
    for (int i = 0; i < (this->items); i++)
    {
        if (i == this->currentItem)
            Ncurses::setStyle(this->hiliteColor);
        else
            Ncurses::setStyle(this->color);

        Ncurses::print(this->item[i]->text,
                       this->item[i]->x,
                       this->item[i]->y);
    }
}
void Menu::update()
{
    this->mouseSelectedItem = false;

    Input* input = Input::getInstance();

    if (input->isKeyDown(KEY_UP))
        this->previous();

    if (input->isKeyDown(KEY_DOWN))
        this->next();

    // if (input->isKeyDown(SDLK_PAGEUP))
    //     this->first();

    // if (input->isKeyDown(SDLK_PAGEDOWN))
    //     this->last();

    // if (input->isMouseInside(this->x, this->y, this->width, this->height))
    // {
    //     for (int i = 0; i < (this->items); i++)
    //     {
    //         Text* text = this->item[i]->text;

    //         if (input->isMouseInside(text->getX(),     text->getY(),
    //                                  text->getWidth(), text->getHeight()))
    //         {
    //             this->item[currentItem]->text->setFont(this->font);
    //             this->currentItem = i;
    //             this->item[currentItem]->text->setFont(this->hiliteColor);

    //             if (input->isMousePressed(SDL_BUTTON_LEFT))
    //                 this->mouseSelectedItem = true;
    //         }
    //     }
    // }
}
void Menu::centralizeText()
{
    // tell everyone to centralize and move the text
    // so it can be right on the middle of the menu.

    for (int i = 0; i < (this->items); i++)
    {
        int newX = (this->width/2) - (this->item[i]->text.length()/2) + (this->x);
        this->item[i]->x = newX;
    }
}
// bool Menu::mouseClickedOnItem()
// {
//     return (this->mouseSelectedItem);
// }

