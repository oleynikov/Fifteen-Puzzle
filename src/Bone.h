#ifndef BONE_H
#define BONE_H

#include <QGraphicsObject>
#include <QPainter>
#include <QPen>
#include <QBrush>

class Bone : public QGraphicsObject
{

    Q_OBJECT

    public:
                            Bone(int number) : number(number)
        {

        }
        int                 getNumber()const
        {

            return this->number;

        }

    protected:
        virtual void        mousePressEvent(QGraphicsSceneMouseEvent* event)
        {

            emit this->clicked(this);

        }
        virtual void        paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
        {

            painter->drawRect(this->boundingRect());
            painter->drawText(this->boundingRect(),Qt::AlignCenter,QString::number(this->number));

        }
        virtual QRectF      boundingRect()const
        {

            return QRectF(5,5,Bone::size-10,Bone::size-10);

        }

    private:
        static const int    size = 50;
        int                 number;

    signals:
        void                clicked(Bone*);

};

#endif // BONE_H
