#ifndef BONEMANAGER_H
#define BONEMANAGER_H

#include <QGraphicsScene>
#include <QGlobal.h>
#include <QObject>
#include <QTime>
#include <QList>
#include <Bone.h>
#include <QMessageBox>

class BoneManager : public QObject
{

    Q_OBJECT

    typedef QList<QPointF>          Cells;
    typedef Cells::iterator         CellsItr;
    typedef QList<Bone*>            Bones;
    typedef Bones::iterator         BonesItr;

    public:
                            BoneManager()
        {

            this->fieldReset();

        }
        QGraphicsScene*     getScene()
                            {

                                return &this->scene;

                            }

    private:
        QGraphicsScene      scene;
        Bones               bones;

        void                fieldReset()
        {

            this->fieldClear();
            this->fieldFill();

        }
        void                fieldClear()
        {

            for (BonesItr itr=this->bones.begin() ; itr!=this->bones.end() ; ++itr)
            {

                delete *itr;

            }

            this->bones.clear();

        }
        void                fieldFill()
        {

            QTime time = QTime::currentTime();
            qsrand((uint)time.msec());
            int boneNumber = 0;
            int boneCount = 0;
            QPointF bonePosition;

            for (int row=0 ; row<4 ; ++row)
            {

                for (int col=0 ; col<4 ; ++col)
                {

                    boneCount++;

                    if (boneCount<16)
                    {

                        do
                        {
                            boneNumber = this->random(1,15);
                        }
                        while (this->getBone(boneNumber) != NULL);

                        bonePosition = QPointF(col*50,row*50);
                        this->addBone(boneNumber,bonePosition);

                    }

                    else
                    {

                        break;

                    }

                }

            }

        }
        Bone*               getBone(int boneNumber)
        {

            Bone* bone = NULL;

            for (BonesItr itr=this->bones.begin() ; itr!=this->bones.end() ; ++itr)
            {

                if ((*itr)->getNumber() == boneNumber)
                {

                    bone = *itr;
                    break;

                }

            }

            return bone;

        }
        Bone*               getBone(QPointF bonePosition)
        {

            Bone* bone = NULL;

            for (BonesItr itr=this->bones.begin() ; itr!=this->bones.end() ; ++itr)
            {

                if ((*itr)->pos() == bonePosition)
                {

                    bone = *itr;
                    break;

                }

            }

            return bone;

        }
        void                addBone(int boneNumber, QPointF position)
        {

            QPointF point;
            Bone* bone = new Bone(boneNumber);
            this->bones.push_back(bone);
            scene.addItem(bone);
            bone->setPos(position);

            QObject::connect(bone,SIGNAL(clicked(Bone*)),this,SLOT(moveBone(Bone*)));

        }
        Cells               getBoneNeighbourCells(Bone* bone)
        {

            Cells cellsAll;
            Cells cellsAvaliable;
            QPointF bonePosition = bone->pos();

            cellsAll.push_back(bonePosition+QPointF(0,50));
            cellsAll.push_back(bonePosition+QPointF(50,0));
            cellsAll.push_back(bonePosition+QPointF(-50,0));
            cellsAll.push_back(bonePosition+QPointF(0,-50));

            for (CellsItr itr=cellsAll.begin() ; itr!=cellsAll.end() ; ++itr)
            {

                QPointF point = *itr;

                if (point.x()>=0 && point.y()>=0 && point.x()<=150 && point.y()<=150)
                {

                    cellsAvaliable.push_back(point);

                }

            }

            return cellsAvaliable;

        }
        QPointF             getBoneNeighbourEmptyCell(Bone* bone)
        {

            Cells cellsAll = this->getBoneNeighbourCells(bone);

            for (BonesItr itr=this->bones.begin() ; itr!=this->bones.end() ; ++itr)
            {

                QPointF bonePoint = (*itr)->pos();
                cellsAll.removeOne(bonePoint);

            }

            if(!cellsAll.empty())
            {

                return cellsAll.first();

            }

            return QPointF(-1,-1);

        }
        int                 random(int low, int high)
        {

            return qrand() % ((high + 1) - low) + low;

        }
        bool                checkWin()
        {

            int boneId = 0;

            for (int row=0 ; row<4 ; ++row)
            {

                for (int col=0 ; col<4 ; ++col)
                {

                    ++boneId;
                    QPointF bonePosition(col*50,row*50);
                    Bone* bone = this->getBone(bonePosition);
                    if (bone && bone->getNumber() != boneId)
                    {

                        return false;

                    }

                }

            }

            return true;

        }

    private slots:
        void                moveBone(Bone* bone)
        {

            QPointF point = this->getBoneNeighbourEmptyCell(bone);

            if (point.x()>=0)
            {

                bone->setX(point.x());
                bone->setY(point.y());

                if (this->checkWin())
                {

                    QMessageBox msgBox;
                    msgBox.setText("You win!");
                    msgBox.exec();

                    this->fieldReset();

                }

            }

        }

};

#endif // BONEMANAGER_H
