#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <ui_WindowGame.h>
#include <BoneManager.h>

class WindowGame : public QWidget
{

    public:
        WindowGame()
        {

            ui.setupUi(this);
            ui.graphicsView->setScene(this->boneManager.getScene());

        }

    private:
        Ui::Form            ui;
        BoneManager         boneManager;




};

#endif // GAME_H
