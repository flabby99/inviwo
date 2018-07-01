/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2016-2018 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#include <modules/animationqt/keyframeqt.h>
#include <modules/animationqt/keyframesequenceqt.h>
#include <modules/animationqt/animationeditorqt.h>
#include <modules/animation/datastructures/keyframe.h>

#include <warn/push>
#include <warn/ignore/all>
#include <QApplication>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QRadialGradient>
#include <warn/pop>

namespace inviwo {

namespace animation {

KeyframeQt::KeyframeQt(Keyframe& keyframe, QGraphicsItem* parent)
    : QGraphicsItem(parent), keyframe_(keyframe) {

    setFlags(ItemIgnoresTransformations | ItemIsMovable | ItemIsSelectable |
             ItemSendsGeometryChanges | ItemSendsScenePositionChanges);
    keyframe_.addObserver(this);

    setPos(mapFromScene(QPointF(keyframe_.getTime().count() * WidthPerSecond, 0)).x(), 0);
    setSelected(keyframe_.isSelected());
}

void KeyframeQt::paint(QPainter* painter, const QStyleOptionGraphicsItem* options,
                       QWidget* widget) {
    IVW_UNUSED_PARAM(options);
    IVW_UNUSED_PARAM(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen = QPen();
    pen.setWidthF(1);
    pen.setCosmetic(true);
    pen.setCapStyle(Qt::RoundCap);
    pen.setStyle(Qt::SolidLine);
    isSelected() ? pen.setColor(QColor(66, 66, 132)) : pen.setColor(QColor(66, 66, 66));
    QRadialGradient Gradient(QPointF(0, 0), TrackHeight / 2 - TrackHeightNudge,
                             QPointF(0, -TrackHeight / 4));
    Gradient.setColorAt(0, isSelected() ? QColor(63, 184, 255) : QColor(220, 220, 220));
    Gradient.setColorAt(1, isSelected() ? QColor(66, 66, 132) : QColor(128, 128, 128));
    QBrush brush = QBrush(Gradient);
    painter->setPen(pen);
    painter->setBrush(brush);
    auto penWidth = pen.widthF();
    int hs = static_cast<int>((KeyframeWidth - penWidth) / 2.0f);
    QPoint p[4] = {{-hs, 0},
                   {0, -TrackHeight / 2 + TrackHeightNudge},
                   {hs, 0},
                   {0, TrackHeight / 2 - TrackHeightNudge}};
    painter->drawPolygon(p, 4);
}

void KeyframeQt::lock() { isEditing_ = true; }

void KeyframeQt::unlock() { isEditing_ = false; }

bool KeyframeQt::islocked() const { return isEditing_; }

void KeyframeQt::onKeyframeTimeChanged(Keyframe*, Seconds) {
    if (!isEditing_) {
        KeyframeQtLock lock(this);
        QPointF newPos(this->parentItem()
                           ->mapFromScene(QPointF(keyframe_.getTime().count() * WidthPerSecond, 0))
                           .x(),
                       0);

        if (newPos != pos()) {
            setPos(newPos);
        }
    }
}

QRectF KeyframeQt::boundingRect() const {
    return QRectF(-KeyframeWidth / 2.0f, -KeyframeHeight / 2.0f, KeyframeWidth, KeyframeHeight);
}

QVariant KeyframeQt::itemChange(GraphicsItemChange change, const QVariant& value) {
    if (change == ItemPositionChange) {
        // Dragging a keyframe to a new time
        qreal xResult = value.toPointF().x();
        if (scene() && !scene()->views().empty() &&
            QApplication::mouseButtons() == Qt::LeftButton) {
            // Snap to a grid depending on the current scale
            // - We get parent coordinates here, and need scene coordinates to snap to something
            // globally
            const qreal xInScene = mapToScene(mapFromParent(xResult, 0.0)).x();
            const qreal xSnappedInScene =
                getSnapTime(xInScene, scene()->views().first()->transform().m11());
            xResult = mapToParent(mapFromScene(std::max(xSnappedInScene, 0.0), 0.0)).x();
        }

        // Restrict vertical movement: y does not change
        return QPointF(xResult, y());

    } else if (change == ItemScenePositionHasChanged) {
        if (!isEditing_) {
            KeyframeQtLock lock(this);
            keyframe_.setTime(Seconds(value.toPointF().x() / static_cast<double>(WidthPerSecond)));
        }
    } else if (change == ItemSelectedChange) {
        keyframe_.setSelected(value.toBool());
    }

    return QGraphicsItem::itemChange(change, value);
}

KeyframeQtLock::KeyframeQtLock(KeyframeQt* keyframe) : keyframe_(keyframe) {
    if (keyframe_) keyframe_->lock();
}

KeyframeQtLock::~KeyframeQtLock() {
    if (keyframe_) keyframe_->unlock();
}

}  // namespace animation

}  // namespace inviwo
